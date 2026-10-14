"""Offline fail-closed gate checks for the federated-bibliotheca PLAN_ONLY extension.

This cannot certify remote permissions, physical Android tests, or confidentiality.
"""
import copy
import json
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONTRACT = ROOT / "config" / "biblio-provider-gates.v1.json"
EXPECTED_PROVIDERS = {
    "GITHUB_PUBLIC", "GITHUB_PRIVATE", "DRIVE_PUBLIC", "DRIVE_PRIVATE",
    "ALPHAXIV", "APK_ARM32", "PRODUCER_CI",
}
VALID_STATES = {"TOKEN_VAZIO", "IMPLEMENTED_UNTESTED", "HOLD", "FAIL", "NOT_APPLICABLE"}
REQUIRED_RECEIPT_A = {"source_provider", "provider_id", "revision_or_exact_commit", "digest_scope", "observed_at", "access"}
REQUIRED_RECEIPT_B = {"actor", "operation", "predecessor", "successor", "evidence", "gate", "rollback", "next_probe"}
FORBIDDEN_FIELDS = {"secret_value", "token_value", "refresh_token", "oauth_token", "password", "private_raw_text", "private_folder_id", "private_file_id"}


def inspect(x):
    errors = []
    if not isinstance(x, dict):
        return ["root must be an object"]
    if x.get("mode") != "PLAN_ONLY" or x.get("claim_allowed") is not False:
        errors.append("plan-only and claim=false required")
    if x.get("no_secret_values") is not True or x.get("private_ids_in_public") is not False or x.get("auto_merge") is not False:
        errors.append("publication, secret and merge boundaries")
    cases = x.get("cases")
    if not isinstance(cases, list) or len(cases) < len(EXPECTED_PROVIDERS):
        return errors + ["missing provider cases"]
    ids = [c.get("id") for c in cases if isinstance(c, dict)]
    if len(ids) != len(cases) or len(set(ids)) != len(ids) or set(ids) != EXPECTED_PROVIDERS:
        errors.append("provider IDs must equal full allowlist uniquely")
    for c in cases:
        if not isinstance(c, dict):
            errors.append("case must be object")
            continue
        if c.get("state") not in VALID_STATES or c.get("state") in {"PASS", "PASS_SCOPED"}:
            errors.append("unproved PASS prohibited")
        if c.get("priority") not in {"P0", "P1", "P2", "P3"}:
            errors.append("priority invalid")
        if not c.get("gate") or not c.get("evidence") or not c.get("rollback"):
            errors.append("gate, evidence, rollback required")
    r = x.get("receipts", {})
    if not isinstance(r, dict) or not REQUIRED_RECEIPT_A.issubset(set(r.get("source", []))) or not REQUIRED_RECEIPT_B.issubset(set(r.get("transition", []))):
        errors.append("double-entry audit incomplete")

    def deny_forbidden(node):
        if isinstance(node, dict):
            if FORBIDDEN_FIELDS & set(node):
                errors.append("secret or private locator field")
            for value in node.values():
                deny_forbidden(value)
        elif isinstance(node, list):
            for value in node:
                deny_forbidden(value)

    deny_forbidden(x)
    return errors


class TestBiblioProviderGates(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.source = json.loads(CONTRACT.read_text(encoding="utf-8"))

    def test_contract_passes_scoped_static_structure(self):
        self.assertEqual(inspect(self.source), [])

    def test_claim_promotion_rejected(self):
        x = copy.deepcopy(self.source)
        x["claim_allowed"] = True
        self.assertTrue(inspect(x))

    def test_absent_private_provider_rejected(self):
        x = copy.deepcopy(self.source)
        x["cases"] = [c for c in x["cases"] if c["id"] != "DRIVE_PRIVATE"]
        self.assertTrue(inspect(x))

    def test_duplicate_provider_rejected(self):
        x = copy.deepcopy(self.source)
        x["cases"][0]["id"] = x["cases"][1]["id"]
        self.assertTrue(inspect(x))

    def test_fake_pass_rejected(self):
        x = copy.deepcopy(self.source)
        x["cases"][0]["state"] = "PASS_SCOPED"
        self.assertTrue(inspect(x))

    def test_secret_field_rejected(self):
        x = copy.deepcopy(self.source)
        x["cases"][0]["oauth_token"] = "synthetic-secret"
        self.assertTrue(inspect(x))

    def test_double_entry_required(self):
        x = copy.deepcopy(self.source)
        x["receipts"]["transition"].remove("rollback")
        self.assertTrue(inspect(x))

    def test_auto_merge_rejected(self):
        x = copy.deepcopy(self.source)
        x["auto_merge"] = True
        self.assertTrue(inspect(x))


if __name__ == "__main__":
    unittest.main()
