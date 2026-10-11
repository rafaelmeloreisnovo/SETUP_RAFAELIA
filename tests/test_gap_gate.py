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


if __name__ == "__main__":
    unittest.main()
