#!/usr/bin/env python3
"""Pure-stdlib offline gate; reports PLAN_ONLY scope and never connects to providers."""
import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
FILE = ROOT / "config" / "setup-catalog.v1.json"
NEEDED = {"id", "drawer", "provider", "kind", "name", "target", "operator",
          "evidence_rule", "depends_on", "privacy_class", "rollback", "manual_only", "state"}
BANNED = {"value", "secret_value", "password", "private_key", "api_key",
          "client_secret", "access_token", "refresh_token", "pat_value"}
DOCS = ("LICENSE", "README.md", "AGENTS.md", "docs/GUIA_RAPIDO.md",
        "docs/PROVIDER_SETTINGS.md", "docs/GOOGLE_DRIVE.md",
        "docs/CONTROLES_E_RECEIPTS.md", "docs/AMEACAS_E_LIMITES.md")
DRAWERS = ("00_ENTRADA", "01_DIREITOS_PRIVACIDADE", "02_QUALIDADE_SEGURANCA",
           "03_COFRE_IDENTIDADE", "04_INTEGRACOES", "05_EVIDENCIAS_OPERACAO")


def validate(data, root=ROOT):
    errors = []
    if not isinstance(data, dict):
        return ["not JSON object"]
    meta = data.get("meta")
    if not isinstance(meta, dict) or meta.get("mode") != "PLAN_ONLY" or meta.get("global_claim_allowed") is not False:
        errors.append("Must remain PLAN_ONLY and global_claim_allowed=false")
    if data.get("schema_version") != "1.0.0" or data.get("drawers") != list(DRAWERS):
        errors.append("Schema or drawers mismatch")
    pins = data.get("source_pins")
    if not isinstance(pins, dict) or any(not str(pins.get(k, "")).startswith("https://docs.google.com/document/d/")
                                           for k in ("start_here", "current_state", "muwrite_ledger")):
        errors.append("Missing canonical Drive pins")
    def recursive_keys(o):
        if isinstance(o, dict):
            for k, v in o.items():
                if k.lower() in BANNED:
                    errors.append("forbidden value field: " + k)
                recursive_keys(v)
        elif isinstance(o, list):
            for v in o:
                recursive_keys(v)
    recursive_keys(data)
    items = data.get("controls")
    if not isinstance(items, list) or not items:
        return errors + ["No controls"]
    ids = set()
    graph = {}
    for i, c in enumerate(items):
        if not isinstance(c, dict):
            errors.append("Nonobject control " + str(i))
            continue
        if set(c) != NEEDED:
            errors.append("Malformed control " + str(i))
        cid = c.get("id")
        if not isinstance(cid, str) or not re.fullmatch(r"[A-Z][A-Z0-9_]{2,80}", cid):
            errors.append("Bad id " + str(i))
            continue
        if cid in ids:
            errors.append("Duplicate id: " + cid)
        ids.add(cid)
        if c.get("drawer") not in DRAWERS or c.get("provider") not in ("github", "google_drive"):
            errors.append("Bad route: " + cid)
        if c.get("kind") not in ("SECRET","SETTING","VARIABLE","RIGHTS","OAUTH_SCOPE","EXECUTION","RECEIPT"):
            errors.append("Bad kind: " + cid)
        if c.get("privacy_class") not in ("metadata","restricted_metadata","confidential"):
            errors.append("Bad privacy: " + cid)
        if c.get("kind") == "SECRET" and (c.get("privacy_class") != "confidential" or not re.fullmatch(r"[A-Z][A-Z0-9_]+", str(c.get("name", "")))):
            errors.append("Unsafe secret metadata: " + cid)
        for field in ("name","target","operator","evidence_rule","rollback"):
            if not isinstance(c.get(field), str) or not c.get(field).strip():
                errors.append("Missing " + field + ": " + cid)
        if c.get("manual_only") is not True or c.get("state") != "TOKEN_VAZIO":
            errors.append("Unproved promotion: " + cid)
        if not isinstance(c.get("depends_on"), list) or any(not isinstance(x, str) for x in c.get("depends_on", [])):
            errors.append("Invalid dependencies: " + cid)
        else:
            graph[cid] = c["depends_on"]
    for n, deps in graph.items():
        for dep in deps:
            if dep not in ids or dep == n:
                errors.append("Dangling/self dependency: " + n + "->" + dep)
    visiting, completed = set(), set()
    def dfs(n):
        if n in visiting:
            errors.append("Cycle: " + n)
            return
        if n in completed:
            return
        visiting.add(n)
        for dep in graph.get(n, []):
            if dep in graph:
                dfs(dep)
        visiting.remove(n)
        completed.add(n)
    for n in graph:
        dfs(n)
    for d in DOCS:
        if not (root / d).is_file():
            errors.append("Missing guide: " + d)
    return errors


def main():
    try:
        data = json.loads(FILE.read_text(encoding="utf-8"))
    except (OSError, ValueError, UnicodeError) as exc:
        print(json.dumps({"status": "FAIL", "error": str(exc)}))
        return 1
    errors = validate(data)
    print(json.dumps({"gate":"SETUP_OFFLINE_PLAN", "status":"PASS_SCOPED" if not errors else "FAIL",
          "controls": len(data.get("controls", [])), "provider_applied": False,
          "secrets_read_or_written":False, "global_claim_allowed":False, "errors":errors},
          indent=2, ensure_ascii=False))
    return int(bool(errors))


if __name__ == "__main__":
    sys.exit(main())
