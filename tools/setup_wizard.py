#!/usr/bin/env python3
"""Read-only setup chooser. Safe for novices: no network, credentials or mutation."""
import argparse
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CATALOG = ROOT / "config" / "setup-catalog.v1.json"
GUIDES = {
    "01_DIREITOS_PRIVACIDADE": "docs/CONTROLES_E_RECEIPTS.md",
    "02_QUALIDADE_SEGURANCA": "docs/PROVIDER_SETTINGS.md",
    "03_COFRE_IDENTIDADE": "docs/PROVIDER_SETTINGS.md",
    "04_INTEGRACOES": "docs/GOOGLE_DRIVE.md",
    "05_EVIDENCIAS_OPERACAO": "docs/CONTROLES_E_RECEIPTS.md",
}


def load():
    return json.loads(CATALOG.read_text(encoding="utf-8"))


def overview(data):
    lines = ["SETUP_RAFAELIA — Escolha sua gaveta (somente leitura):"]
    for i, drawer in enumerate(data["drawers"], start=0):
        amount = sum(x["drawer"] == drawer for x in data["controls"])
        lines.append(f"{i} — {drawer} ({amount} controles)")
    lines.append("Para uma visão geral: docs/GUIA_RAPIDO.md")
    lines.append("Nenhuma configuração de provedor é aplicada por este assistente.")
    return "\n".join(lines)


def view_control(data, control_id):
    match = next((x for x in data["controls"] if x["id"] == control_id), None)
    if match is None:
        raise ValueError("Controle não encontrado; use --list para ver IDs.")
    guide = GUIDES.get(match["drawer"], "docs/GUIA_RAPIDO.md")
    return "\n".join([
        "Controle: " + match["id"],
        "Gaveta: " + match["drawer"],
        "Plataforma: " + match["provider"],
        "Tipo: " + match["kind"],
        "Destino: " + match["target"],
        "Operador exigido: " + match["operator"],
        "Estado real: TOKEN_VAZIO (NÃO OBSERVADO)",
        "Prova que ainda precisa existir: " + match["evidence_rule"],
        "Reversão: " + match["rollback"],
        "Dependências: " + (", ".join(match["depends_on"]) or "nenhuma"),
        "Guia: " + guide,
        "AÇÃO: procurar operador autorizado; nunca inserir chaves aqui."
    ])


def main(argv=None):
    p = argparse.ArgumentParser(description="Gavetas de setup; offline e sem credenciais")
    p.add_argument("--list", action="store_true", help="Listar os IDs de controles")
    p.add_argument("--control", help="Detalhes de um ID do catálogo")
    p.add_argument("--drawer", help="Listar controles de uma gaveta exata")
    a = p.parse_args(argv)
    data = load()
    if a.control:
        try:
            print(view_control(data, a.control))
            return 0
        except ValueError as e:
            print(str(e))
            return 2
    if a.drawer:
        if a.drawer not in data["drawers"]:
            print("Gaveta desconhecida. Use sem argumentos para ver todas.")
            return 2
        for x in data["controls"]:
            if x["drawer"] == a.drawer:
                print(x["id"] + " — " + x["kind"] + " — TOKEN_VAZIO")
        return 0
    if a.list:
        for x in data["controls"]:
            print(x["id"] + " — " + x["drawer"])
        return 0
    print(overview(data))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
