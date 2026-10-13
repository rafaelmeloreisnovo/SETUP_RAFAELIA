import importlib.util
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CLI = ROOT / "catalog.py"
SOURCE = "0123456789abcdef0123456789abcdef01234567"
AUTHOR = "https://github.com/rafaelmeloreisnovo"
TOOLHASH = "a" * 64

class CatalogTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.path = Path(self.temp.name) / "artifact.apk"
        self.path.write_bytes(b"PK\x03\x04NOT_AN_ACTUAL_APK")
        self.receipt = Path(self.temp.name) / "receipt.json"

    def cli(self, *args):
        return subprocess.run([sys.executable, str(CLI), *map(str, args)],
                              capture_output=True, text=True)

    def record(self):
        return self.cli("record", self.path, "--source-commit", SOURCE,
                        "--author-uri", AUTHOR, "--package-id", "org.example.demo",
                        "--toolchain-name", "gcc-host-only",
                        "--toolchain-sha256", TOOLHASH, "--out", self.receipt)

    def verify(self, *args):
        return self.cli("verify", self.path, "--manifest", self.receipt,
                        "--trusted-author-uri", AUTHOR,
                        "--trusted-source-commit", SOURCE, *args)

    def test_record_and_hash_only_verification(self):
        self.assertEqual(self.record().returncode, 0)
        r = self.verify("--hash-only")
        self.assertEqual(r.returncode, 0)
        self.assertIn("NOT_AUTHENTICATED", r.stdout)

    def test_signature_missing_cannot_promote(self):
        self.assertEqual(self.record().returncode, 0)
        self.assertEqual(self.verify().returncode, 3)

    def test_payload_tampering_fails(self):
        self.assertEqual(self.record().returncode, 0)
        self.path.write_bytes(b"X" + self.path.read_bytes()[1:])
        self.assertEqual(self.verify("--hash-only").returncode, 2)

    def test_source_identity_mismatch_fails(self):
        self.assertEqual(self.record().returncode, 0)
        self.assertEqual(self.cli("verify", self.path, "--manifest", self.receipt,
                                 "--trusted-author-uri", "https://example.org",
                                 "--trusted-source-commit", SOURCE,
                                 "--hash-only").returncode, 2)

    def test_forged_signature_status_fails(self):
        self.assertEqual(self.record().returncode, 0)
        payload = json.loads(self.receipt.read_text())
        payload["signature_verification"] = "PASS"
        self.receipt.write_text(json.dumps(payload))
        self.assertEqual(self.verify("--hash-only").returncode, 2)

    def test_duplicate_json_key_fails(self):
        self.assertEqual(self.record().returncode, 0)
        text = self.receipt.read_text()
        text = text.replace('"claim_allowed": false,',
                            '"claim_allowed": false, "claim_allowed": false,')
        self.receipt.write_text(text)
        self.assertEqual(self.verify("--hash-only").returncode, 2)

    def test_existing_receipt_not_overwritten(self):
        self.assertEqual(self.record().returncode, 0)
        self.assertEqual(self.record().returncode, 2)

    def test_symlink_input_rejected(self):
        self.assertEqual(self.record().returncode, 0)
        link = Path(self.temp.name) / "symlink.apk"
        link.symlink_to(self.path)
        self.assertEqual(self.cli("record", link, "--source-commit", SOURCE,
                                  "--author-uri", AUTHOR, "--package-id", "abc",
                                  "--toolchain-name", "gcc",
                                  "--toolchain-sha256", TOOLHASH,
                                  "--out", Path(self.temp.name) / "other.json").returncode, 2)

if __name__ == "__main__":
    unittest.main()
