import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from setup_wizard import load, overview, view_control  # noqa: E402


class WizardTests(unittest.TestCase):
    def test_offline_overview_has_six_drawers(self):
        data = load()
        self.assertEqual(len(data["drawers"]), 6)
        self.assertIn("Nenhuma configuração", overview(data))

    def test_control_displays_unknown_and_route(self):
        out = view_control(load(), "GH_MAIN_RULESET")
        self.assertIn("TOKEN_VAZIO", out)
        self.assertIn("docs/PROVIDER_SETTINGS.md", out)

    def test_secret_shows_name_not_value(self):
        out = view_control(load(), "CODESCAN_AUTH")
        self.assertIn("Tipo: SECRET", out)
        self.assertNotIn("Senha:", out)
        self.assertIn("NÃO OBSERVADO", out)

    def test_unknown_control_rejected(self):
        with self.assertRaises(ValueError):
            view_control(load(), "NOT_CONFIGURED_ID")


if __name__ == "__main__":
    unittest.main()
