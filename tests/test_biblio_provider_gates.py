"""Offline fail-closed gate checks for the federated-bibliotheca PLAN_ONLY extension.

This cannot certify remote permissions, physical Android tests, or confidentiality.
"""
import copy
import json
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CONTRACT = ROOT / "config" / "biblio-provider-gates.v1.json"
from validate_biblio_provider_gates import inspect


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


    def test_missing_falsifier_rejected(self):
        x = copy.deepcopy(self.source)
        x["cases"][0]["falsifier"] = ""
        self.assertTrue(inspect(x))

    def test_authority_required(self):
        x = copy.deepcopy(self.source)
        x["cases"][0].pop("authority")
        self.assertTrue(inspect(x))

    def test_arm32_physical_false_claim_rejected(self):
        x = copy.deepcopy(self.source)
        x["runtime_claim_boundary"]["arm32_device"] = "PASS"
        self.assertTrue(inspect(x))

    def test_arm32_implemented_not_device_pass(self):
        x = copy.deepcopy(self.source)
        next(c for c in x["cases"] if c["id"] == "APK_ARM32")["state"] = "IMPLEMENTED_UNTESTED"
        self.assertTrue(inspect(x))

    def test_privacy_priority_regression(self):
        x = copy.deepcopy(self.source)
        next(c for c in x["cases"] if c["id"] == "ALPHAXIV")["priority"] = "P3"
        self.assertTrue(inspect(x))

    def test_draft_is_not_enforcement(self):
        x = copy.deepcopy(self.source)
        x["gate_evidence_policy"]["public_pr_draft_is_gate"] = True
        self.assertTrue(inspect(x))


if __name__ == "__main__":
    unittest.main()
