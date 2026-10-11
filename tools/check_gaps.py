#!/usr/bin/env python3
"""Read-only gap topology gate. CI PASS here does not enforce provider rules."""
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
STATES = {"TOKEN_VAZIO", "PARTIAL_OBSERVED", "OPEN_OBSERVED"}
REASONS_REQUIRED = {"OPEN_OBSERVED", "PARTIAL_OBSERVED"}


def validate(catalog, register):
    errors = []
    if not isinstance(catalog, dict) or not isinstance(register, dict):
        return ["root not object"]
    if register.get("schema") != "RAFAELIA_TYPED_GAPS_V1":
        errors.append("invalid register schema")
    if register.get("claim_allowed") is not False:
        errors.append("global claim must be false")
    if register.get("scope") != "OBSERVATION_ONLY_NO_PROVIDER_WRITE":
        errors.append("wrong evidence scope")
    controls = catalog.get("controls")
    rows = register.get("items")
    if not isinstance(controls, list) or not isinstance(rows, list):
        return errors + ["controls or items missing"]
    ids = [x.get("id") for x in controls if isinstance(x, dict)]
    names = [x.get("control_id") for x in rows if isinstance(x, dict)]
    if len(ids) != len(set(ids)) or len(names) != len(set(names)):
        errors.append("duplicate control or gap")
    if set(ids) != set(names) or len(ids) != len(names):
        errors.append("gap coverage mismatch")
    for row in rows:
        if not isinstance(row, dict):
            errors.append("gap must be object")
            continue
        if set(row) != {"control_id", "state", "cause", "source", "minimum_falsifier", "decision"}:
            errors.append("unknown/missing fields for " + str(row.get("control_id")))
        key = str(row.get("control_id"))
        if row.get("state") not in STATES:
            errors.append("unsupported/false promotion for " + key)
        if row.get("decision") != "HOLD":
            errors.append("promotion not authorized for " + key)
        for field in ("cause", "minimum_falsifier"):
            if not isinstance(row.get(field), str) or not row[field].strip():
                errors.append("untyped " + field + " for " + key)
        ref = row.get("source")
        if row.get("state") in REASONS_REQUIRED and (
                not isinstance(ref, str) or not ref.startswith(
                    "https://github.com/rafaelmeloreisnovo/")):
            errors.append("observed source not authorized for " + key)
        if row.get("state") == "TOKEN_VAZIO" and ref is not None:
            errors.append("unknown state falsely bound to source for " + key)
    byid = {x.get("control_id"): x for x in rows if isinstance(x, dict)}
    for key in ("GH_MAIN_RULESET", "GH_INDEPENDENT_REVIEW",
                "SETUP_MAIN_RULESET", "SETUP_INDEPENDENT_REVIEW"):
        if byid.get(key, {}).get("state") != "OPEN_OBSERVED":
            errors.append("unresolved observed security gap silently closed: " + key)
    if byid.get("GH_BYPASS_AUDIT", {}).get("state") != "PARTIAL_OBSERVED":
        errors.append("bypass identity not independently proven")
    return errors


def main():
    try:
        cat = json.loads((ROOT / "config/setup-catalog.v1.json").read_text(encoding="utf-8"))
        gaps = json.loads((ROOT / "config/gap-register.v1.json").read_text(encoding="utf-8"))
        errors = validate(cat, gaps)
        count = len(gaps.get("items", []))
    except (OSError, ValueError, TypeError) as exc:
        errors, count = [str(exc)], 0
    print(json.dumps({
        "gate": "RAFAELIA_TYPED_GAP_GATE_V1",
        "status": "PASS_SCOPED" if not errors else "FAIL",
        "total_controls": count,
        "provider_enforcement_proven": False,
        "claim_allowed": False,
        "errors": errors
    }, indent=2, ensure_ascii=False))
    return int(bool(errors))


if __name__ == "__main__":
    sys.exit(main())
