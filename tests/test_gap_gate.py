import unittest
from pathlib import Path

class TestGapGate(unittest.TestCase):
    def test_registry_exists(self):
        root = Path(__file__).resolve().parents[1]
        self.assertTrue((root / 'config/gap-register.v1.json').is_file())
