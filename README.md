# SETUP_RAFAELIA 🗂️ — Casa de Configuração

**Para iniciantes, mantenedores e auditores.** Cada gaveta responde: o que é, quem pode configurar, onde, como comprovar e como reverter.

> **PLAN_ONLY:** este repositório prepara a configuração, mas **não cria chaves, ativa proteções, dá acesso ao Drive, instala APK ou certifica produção**. Sem readback do provedor, a situação é `TOKEN_VAZIO`, não `PASS`.

| Preciso de... | Comece em... |
| --- | --- |
| Entender sem termos técnicos | [Guia de 10 minutos](docs/GUIA_RAPIDO.md) |
| Configurar GitHub, rulesets, reviewers, secrets, vars e CodeScan | [Settings passo a passo](docs/PROVIDER_SETTINGS.md) |
| Integrar Google Drive sem expor dados | [Guia Drive](docs/GOOGLE_DRIVE.md) |
| Auditar direitos, Zero Trust, risco, evidência e rollback | [Matriz e receipts](docs/CONTROLES_E_RECEIPTS.md) |
| Fazer revisão de ameaças | [Privacidade e segurança](docs/AMEACAS_E_LIMITES.md) |

## Seis gavetas
- **00 Entrada** → objetivo, contexto, dono e passos para pessoas leigas.
- **01 Direitos/Privacidade** → licença, consentimento, minimização, direitos de terceiros.
- **02 Qualidade/Segurança** → CI, CodeQL, CodeScan, revisores, proteção de `main`.
- **03 Cofre/Identidade** → Secrets, Variables, PATs, OIDC, rotação, expiração.
- **04 Integrações** → GitHub ↔ Google Drive ↔ repositório produtor, contas qualificadas.
- **05 Evidências/Operação** → teste negativo, readback, receipt, falha segura e rollback.

## Fluxo
`INTENT → CURRENT_STATE → SOURCE_MIN → AUTHORITY → PLAN → HUMAN_APPLY → READBACK → FALSIFIER → RECEIPT → DECISION`

`SOURCE ≠ ARTIFACT ≠ EXECUTION ≠ EVIDENCE ≠ CLAIM` · `TOKEN_VAZIO ≠ 0` · `IMPLEMENTED_UNTESTED ≠ PASS`.

### Assistente de gavetas (sem tocar em settings)
Execute `python3 tools/setup_wizard.py` para ver as seis gavetas ou `python3 tools/setup_wizard.py --control GH_MAIN_RULESET` para obter o cartão de prova do controle. Para listar todos: `--list`. Nenhuma entrada de senha é solicitada.

### Verificação somente de leitura
Exige Python 3.11+ sem bibliotecas externas:
```bash
python3 tools/validate_setup.py
python3 -m unittest discover -s tests -v
```
A CI testa apenas a **estrutura do plano**. Não consulta APIs, não lê ou escreve secrets e não realiza provider settings.

### Rotas canônicas (Google Drive, apenas ponteiros)
- [START HERE V2.2](https://docs.google.com/document/d/1T4OIN2RC1I-KFsdaQTCqieeZUGYBXissWrdrZMG0DN8/edit)
- [HOTSTATE V4.5](https://docs.google.com/document/d/1d0J5SkF2S2emBq6jLuTkhYVaSOQiWazJWyRCTMJ-iRI/edit)
- [μWRITE ledger](https://docs.google.com/document/d/1NEU7lg7iUZf1u7lX-SEKuc-knSDGlF5gOx7qzXcJ1yc/edit)

[Catálogo tipado](config/setup-catalog.v1.json) — nomes de controls/credentials são **metadados**, jamais provas de presença. O repositório produtor governa implementação, o Drive governa memória e recibos. **Licença MIT original permanece preservada.**

## Ω V3 — Inventário transversal dos núcleos já existentes (auditoria documental)

A [auditoria freestanding da federação de 13/10/2026](docs/FEDERATION_EXISTING_FREESTANDING_EVIDENCE_20261013.md) cruza produtores reais (RafPolimata, RafGitTools, Vectras, Termux App/Packages/API, Frida, Mapa, Gaia, RMRCTI), geometria NEON de 4096 **bits versus bytes**, custódia e proteção do GitHub. **PLAN_ONLY**, não move/recria núcleos, não legitima binários físicos nem altera direitos e settings; inconsistências e fontes ausentes permanecem tipadas.

### Ω V4 — Biblioteca Drive no Android (plano, sem ativação)

O [contrato de pipeline Android L0/L1 → Google Drive](docs/DRIVE_LIBRARY_ANDROID_L0_HOST_CONTRACT_V1.md) fixa os produtores existentes, separa núcleo freestanding do broker OAuth/ART, tipa BitGhost/CRC/NUMA/GPU e exige readback, falsificadores e reconstrução exata antes de qualquer promoção. **PLAN_ONLY, sem varredura privada e sem gravação de dados ou tokens no GitHub.**

## Ω V2 — Lacunas tipadas e proveniência

A [matriz de lacunas](config/gap-register.v1.json) liga **18 controles** ao motivo de incerteza, à fonte observada e ao experimento mínimo que pode falsificá-la. O [guia de proveniência](docs/PROVENIENCIA_E_LACUNAS.md) explica os limites dos fatos do GitHub e o [validador offline](tools/check_gaps.py) verifica a cobertura 1:1.

```bash
python3 tools/check_gaps.py
python3 -m unittest discover -s tests -v
```

**Atenção:** `OPEN_OBSERVED` não significa resolvido. A configuração de proteção do SETUP e do Mapa ainda requer operador nativo, revisão independente, readback e teste negativo do servidor. CI verde aqui valida só o modelo de lacunas.
