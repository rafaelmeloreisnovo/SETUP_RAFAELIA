import copy
import json
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from validate_setup import validate

DATA = json.loads((ROOT / "config" / "setup-catalog.v1.json").read_text(encoding="utf-8"))


class TestOfflinePlan(unittest.TestCase):
    def test_valid_catalog(self):
        self.assertEqual(validate(DATA), [])

    def test_global_claim_rejected(self):
        d = copy.deepcopy(DATA); d["meta"]["global_claim_allowed"] = True
        self.assertTrue(validate(d))

    def test_secret_value_rejected(self):
        d = copy.deepcopy(DATA); d["controls"][0]["secret_value"] = "fake"
        self.assertTrue(validate(d))

    def test_unproved_status_rejected(self):
        d = copy.deepcopy(DATA); d["controls"][0]["state"] = "PASS"
        self.assertTrue(validate(d))

    def test_automated_mutation_rejected(self):
        d = copy.deepcopy(DATA); d["controls"][0]["manual_only"] = False
        self.assertTrue(validate(d))

    def test_duplicate_rejected(self):
        d = copy.deepcopy(DATA); d["controls"][1]["id"] = d["controls"][0]["id"]
        self.assertTrue(validate(d))

    def test_dangling_dependency_rejected(self):
        d = copy.deepcopy(DATA); d["controls"][0]["depends_on"] = ["UNKNOWN"]
        self.assertTrue(validate(d))

    def test_cycle_rejected(self):
        d = copy.deepcopy(DATA); d["controls"][0]["depends_on"] = ["GH_BYPASS_AUDIT"]
        self.assertTrue(validate(d))

    def test_secret_must_be_confidential(self):
        d = copy.deepcopy(DATA)
        next(x for x in d["controls"] if x["kind"] == "SECRET")["privacy_class"] = "metadata"
        self.assertTrue(validate(d))

    def test_docs_mandatory(self):
        with tempfile.TemporaryDirectory() as folder:
            self.assertTrue(validate(DATA, Path(folder)))

    def test_ambiguous_input_rejected(self):
        self.assertTrue(validate([]))


if __name__ == "__main__":
    unittest.main()
