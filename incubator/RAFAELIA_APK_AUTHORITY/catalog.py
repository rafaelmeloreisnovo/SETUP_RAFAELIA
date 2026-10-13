#!/usr/bin/env python3
"""RAFAELIA catalog v0: host-side hash inventory only. NOT signature verification."""
import argparse
import hashlib
import json
import os
import re
import sys
from pathlib import Path

SCHEMA = "rafaelia.artifact-hash-catalog.v0"
HEX40 = re.compile(r"[0-9a-f]{40}\Z")
HEX64 = re.compile(r"[0-9a-f]{64}\Z")
MAX_MANIFEST = 262144

class Invalid(ValueError):
    pass

def guarded_file(path):
    p = Path(path)
    if p.is_symlink() or not p.is_file():
        raise Invalid("missing, symlink or non-regular input")
    return p

def digest(path):
    h = hashlib.sha256()
    with guarded_file(path).open("rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()

def require_hex(value, pattern, key):
    if not pattern.fullmatch(value):
        raise Invalid("invalid " + key)
    return value

def pair_unique(pairs):
    obj = {}
    for k, v in pairs:
        if k in obj:
            raise Invalid("duplicate JSON key: " + k)
        obj[k] = v
    return obj

def record(args):
    p = guarded_file(args.artifact)
    if not p.name or p.name in (".", ".."):
        raise Invalid("invalid name")
    if not re.fullmatch(r"[A-Za-z0-9_.-]{1,200}", args.package_id):
        raise Invalid("invalid package-id")
    require_hex(args.source_commit, HEX40, "source commit")
    require_hex(args.toolchain_sha256, HEX64, "toolchain hash")
    if not args.author_uri.startswith("https://") or len(args.author_uri) > 1000:
        raise Invalid("author URI must be HTTPS")
    if not re.fullmatch(r"[A-Za-z0-9_.:-]{1,128}", args.toolchain_name):
        raise Invalid("invalid toolchain")
    out = Path(args.out)
    if out.exists() or out.is_symlink() or out.resolve() == p.resolve():
        raise Invalid("output exists or overlaps input")
    manifest = {
        "schema": SCHEMA,
        "source": {"git_commit": args.source_commit},
        "author": {"public_uri": args.author_uri},
        "toolchain": {"name": args.toolchain_name, "sha256": args.toolchain_sha256},
        "artifact": {"name": p.name, "package_id": args.package_id,
                     "sha256": digest(p), "size": p.stat().st_size},
        "signature_verification": "TOKEN_VAZIO_NOT_IMPLEMENTED",
        "claim_allowed": False,
    }
    with out.open("x", encoding="utf-8") as f:
        json.dump(manifest, f, sort_keys=True, indent=2, ensure_ascii=True)
        f.write("\n")
    return "RECORDED_HASH_ONLY"

def verify(args):
    p = guarded_file(args.artifact)
    mp = guarded_file(args.manifest)
    if mp.stat().st_size > MAX_MANIFEST:
        raise Invalid("manifest too large")
    with mp.open("r", encoding="utf-8") as f:
        m = json.load(f, object_pairs_hook=pair_unique)
    if not isinstance(m, dict) or set(m) != {
        "schema", "source", "author", "toolchain", "artifact",
        "signature_verification", "claim_allowed"
    }:
        raise Invalid("unexpected schema fields")
    if m["schema"] != SCHEMA or m["claim_allowed"] is not False:
        raise Invalid("untrusted status")
    if m["signature_verification"] != "TOKEN_VAZIO_NOT_IMPLEMENTED":
        raise Invalid("forged signer status")
    if not isinstance(m["author"], dict) or m["author"].get("public_uri") != args.trusted_author_uri:
        raise Invalid("untrusted author identity")
    if not isinstance(m["source"], dict) or m["source"].get("git_commit") != args.trusted_source_commit:
        raise Invalid("source commit mismatch")
    require_hex(args.trusted_source_commit, HEX40, "trusted source")
    a = m["artifact"]
    if not isinstance(a, dict) or set(a) != {"name", "package_id", "sha256", "size"}:
        raise Invalid("artifact schema mismatch")
    if a["name"] != p.name or type(a["size"]) is not int or a["size"] != p.stat().st_size:
        raise Invalid("artifact metadata mismatch")
    require_hex(a["sha256"], HEX64, "artifact sha256")
    if not hashlib.compare_digest(a["sha256"], digest(p)) if hasattr(hashlib, "compare_digest") else a["sha256"] != digest(p):
        raise Invalid("artifact hash mismatch")
    if not isinstance(m["toolchain"], dict) or set(m["toolchain"]) != {"name", "sha256"}:
        raise Invalid("toolchain schema mismatch")
    require_hex(m["toolchain"]["sha256"], HEX64, "toolchain sha256")
    if not args.hash_only:
        return "NOT_VERIFIED_SIGNER", 3
    return "HASH_ONLY_MATCH_NOT_AUTHENTICATED", 0

def main(argv=None):
    parser = argparse.ArgumentParser()
    sub = parser.add_subparsers(dest="command", required=True)
    r = sub.add_parser("record")
    r.add_argument("artifact")
    for flag in ("source-commit", "author-uri", "package-id", "toolchain-name",
                 "toolchain-sha256", "out"):
        r.add_argument("--" + flag, required=True)
    v = sub.add_parser("verify")
    v.add_argument("artifact")
    v.add_argument("--manifest", required=True)
    v.add_argument("--trusted-author-uri", required=True)
    v.add_argument("--trusted-source-commit", required=True)
    v.add_argument("--hash-only", action="store_true")
    args = parser.parse_args(argv)
    try:
        if args.command == "record":
            print(record(args))
            return 0
        result, code = verify(args)
        print(result)
        return code
    except (Invalid, OSError, UnicodeError, json.JSONDecodeError, TypeError, KeyError) as e:
        print("FAIL_CLOSED: " + str(e), file=sys.stderr)
        return 2

if __name__ == "__main__":
    sys.exit(main())
