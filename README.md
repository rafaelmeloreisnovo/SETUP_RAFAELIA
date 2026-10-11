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
