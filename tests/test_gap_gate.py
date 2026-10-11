import json
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from check_gaps import validate

C = json.loads((ROOT / "config/setup-catalog.v1.json").read_text())
G = json.loads((ROOT / "config/gap-register.v1.json").read_text())


class TestGapGate(unittest.TestCase):
    def test_baseline(self):
        self.assertEqual(validate(C, G), [])

    def test_missing_record(self):
        incomplete = dict(G, items=G["items"][:-1])
        self.assertTrue(validate(C, incomplete))

    def test_invalid_status(self):
        rows = [dict(item) for item in G["items"]]
        rows[0]["state"] = "UNVERIFIED_PASS"
        self.assertTrue(validate(C, dict(G, items=rows)))

    def test_dangling_reference(self):
        rows = [dict(item) for item in G["items"]]
        rows[0]["source"] = "https://invalid.example/receipt"
        self.assertTrue(validate(C, dict(G, items=rows)))

    def test_empty_falsifier(self):
        rows = [dict(item) for item in G["items"]]
        rows[0]["minimum_falsifier"] = ""
        self.assertTrue(validate(C, dict(G, items=rows)))



if __name__ == "__main__":
    unittest.main()
