#!/usr/bin/env python3
"""Offline provider-boundary validator for SETUP_RAFAELIA (not a live permissions probe).

All results are scoped to a local plan and cannot confer provider authority.
No network, secrets, or writes beyond stdout. Failure means malformed plan.
"""
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "config/biblio-provider-gates.v1.json"
PROVIDERS = {
    "GITHUB_PUBLIC", "GITHUB_PRIVATE", "DRIVE_PUBLIC", "DRIVE_PRIVATE",
    "ALPHAXIV", "APK_ARM32", "PRODUCER_CI",
}
PRIVATE_GATES = {"GITHUB_PRIVATE", "DRIVE_PUBLIC", "DRIVE_PRIVATE", "ALPHAXIV"}
STATES = {"TOKEN_VAZIO", "IMPLEMENTED_UNTESTED", "HOLD", "FAIL", "NOT_APPLICABLE"}
SOURCE_FIELDS = {"source_provider", "provider_id", "revision_or_exact_commit",
                 "digest_scope", "observed_at", "access"}
TRANSITION_FIELDS = {"actor", "operation", "predecessor", "successor",
                     "evidence", "gate", "rollback", "next_probe"}
BANNED = {
    "secret_value", "token_value", "refresh_token", "oauth_token", "password",
    "private_raw_text", "private_folder_id", "private_file_id", "access_token",
    "private_key", "conversation_id", "message_id", "raw_conversation",
}


def inspect(plan: dict) -> list[str]:
    errors = []
    if not isinstance(plan, dict):
        return ["ROOT_NOT_OBJECT"]
    if plan.get("schema") != "rafaelia.setup.federated-biblio-gates/v1":
        errors.append("SCHEMA_MISMATCH")
    if plan.get("mode") != "PLAN_ONLY" or plan.get("claim_allowed") is not False:
        errors.append("UNAUTHORIZED_CLAIM")
    for key, expected in (("no_secret_values", True),
                          ("private_ids_in_public", False), ("auto_merge", False)):
        if plan.get(key) is not expected:
            errors.append("UNSAFE_POLICY:" + key)
    cases = plan.get("cases")
    if not isinstance(cases, list):
        return errors + ["CASES_NOT_LIST"]
    ids = []
    for index, case in enumerate(cases):
        if not isinstance(case, dict):
            errors.append("CASE_NOT_OBJECT:%d" % index)
            continue
        name = case.get("id")
        ids.append(name)
        if case.get("state") not in STATES:
            errors.append("FALSE_OR_UNTYPED_PASS:%s" % name)
        if case.get("priority") not in {"P0", "P1", "P2", "P3"}:
            errors.append("INVALID_PRIORITY:%s" % name)
        if not all(isinstance(case.get(k), str) and case[k].strip()
                   for k in ("access", "gate", "evidence", "rollback", "falsifier", "authority")):
            errors.append("MISSING_OPERATIONAL_GATE:%s" % name)
        if name in PRIVATE_GATES and case.get("priority") != "P0":
            errors.append("PRIVACY_PRIORITY_REGRESSION:%s" % name)
    if len(ids) != len(PROVIDERS) or set(ids) != PROVIDERS:
        errors.append("PROVIDER_SET_OR_DUPLICATE_MISMATCH")

    receipts = plan.get("receipts", {})
    if not isinstance(receipts, dict):
        errors.append("RECEIPTS_MISSING")
    else:
        a = receipts.get("source")
        b = receipts.get("transition")
        if not isinstance(a, list) or not SOURCE_FIELDS.issubset(set(a)):
            errors.append("CUSTODY_SOURCE_ENTRY_INCOMPLETE")
        if not isinstance(b, list) or not TRANSITION_FIELDS.issubset(set(b)):
            errors.append("CUSTODY_TRANSITION_ENTRY_INCOMPLETE")

    runtime = plan.get("runtime_claim_boundary", {})
    if not isinstance(runtime, dict) or runtime.get("claim_allowed") is not False:
        errors.append("RUNTIME_CLAIM_UNSCOPED")
    elif any(runtime.get(k) != "TOKEN_VAZIO"
             for k in ("apk_artifact", "arm32_device", "host_adapter")):
        errors.append("UNPROVED_DEVICE_STATE_PROMOTED")
    arm = next((r for r in cases if isinstance(r, dict) and r.get("id") == "APK_ARM32"), {})
    if arm.get("state") != "TOKEN_VAZIO" or arm.get("gap_type") != "MISSING_PHYSICAL_EXECUTION_RECEIPT":
        errors.append("PHYSICAL_PROOF_MISSING_NOT_TYPED")
    evidence = plan.get("gate_evidence_policy", {})
    if not isinstance(evidence, dict) or evidence.get("claim_allowed") is not False:
        errors.append("EXTERNAL_EVIDENCE_CLAIM_INVALID")
    elif evidence.get("public_pr_draft_is_gate") is not False:
        errors.append("PUBLIC_PR_DRAFT_IS_NOT_ENFORCEMENT")

    def walk(node):
        if isinstance(node, dict):
            for name, value in node.items():
                if name.lower() in BANNED:
                    errors.append("SECRET_OR_PRIVATE_LOCATOR:%s" % name)
                walk(value)
        elif isinstance(node, list):
            for value in node:
                walk(value)
    walk(plan)
    return sorted(set(errors))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("path", nargs="?", type=Path, default=SOURCE)
    args = parser.parse_args()
    try:
        plan = json.loads(args.path.read_text(encoding="utf-8"))
        errors = inspect(plan)
    except (OSError, ValueError) as exc:
        errors = ["READ_OR_PARSE_ERROR:" + str(exc)]
    print(json.dumps({
        "schema": "rafaelia.setup.offline-biblio-validation/v1",
        "claim_allowed": False,
        "offline_structure": "PASS_SCOPED" if not errors else "FAIL",
        "provider_enforcement": "HOLD_LIVE_READBACK_REQUIRED",
        "review": "TOKEN_VAZIO_INDEPENDENT_REVIEW",
        "android_arm32": "TOKEN_VAZIO_PHYSICAL_RECEIPT",
        "errors": errors,
    }, ensure_ascii=False, sort_keys=True))
    return 0 if not errors else 1


if __name__ == "__main__":
    raise SystemExit(main())
