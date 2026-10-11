# GitHub: configurações de qualidade e segurança

**Manual, autorizado e reversível.** Os menus variam por plano/tipo de conta. Este SETUP não altera settings de provedores automaticamente.

## Gaveta 02 — proteção de `main`
1. Com administrador legítimo: repositório → **Settings → Rules → Rulesets** (ou Branches, se aplicável).
2. Validar alvo da regra (ex.: `main`; `~DEFAULT_BRANCH` não garante outras branches).
3. Inventariar cada ator com `bypass_mode=always`: identidade, justificativa, prazo e redução de privilégio.
4. Planejar PR obrigatório, **ao menos uma aprovação elegível e independente**, descarte de aprovação obsoleta após push, resolução de comentários e checks com nomes que realmente existam.
5. `CODEOWNERS` que nomeia somente o autor **não** é revisão independente.
6. Avaliar impacto/lockout antes de ativar (modo Evaluate se disponível); ativar somente com autorização.
7. **Readback** da branch+ruleset; prova negativa: pull request inocente, sem aprovação, deve ser bloqueado pelo servidor **sem consumar merge**. Comparar `main` antes/depois.

## Gaveta 03 — Secrets / Variables / Keys
GitHub → **Settings → Secrets and variables → Actions** ou **Environments** para segredos específicos por ambiente. Nunca registrar **valores** em git, issues, chats, logs ou Drive.

- `CODESCAN_AUTH_TOKEN`, `CODESCAN_ORGANIZATION_KEY`, `CODESCAN_PROJECT_KEY`: nomes declarados pelo produtor Mapa; **não provam existência**. Após configurar no escopo correto, provar preflight `PRESENT` sem valor + análise + SARIF **não vazio** + upload e execução terminal no HEAD exato.
- `PAT_AGENTS` e `PAT_ACTIONS`: nomes históricos no RafGitTools, classificados como **candidatos legados** até provar necessidade, dono, permissões e vencimento. Não copiar entre repos.
- **Variables** não são cofres; opções públicas apenas. `RAFAELIA_SETUP_MODE=PLAN_ONLY` é sugestão, não variável efetivamente criada.
- Preferir `GITHUB_TOKEN` com permissões explícitas, GitHub App ou OIDC de curta duração a PAT amplo. Revogação, expiração, rotação e teste de menor privilégio fazem parte do fechamento.

## Checks de qualidade e release
Separar: CI unitária, lint, testes negativos, CodeQL, CodeScan, secret scan, SBOM/licenças, ações pinadas por SHA, auditoria de permissões, assinatura de artefato e aprovação humana. Cada gate tem **evidência própria**. Não substituir scanner falho por outro scanner aprovado. Não desligar checks para tornar a branch verde.

## Recibo mínimo
`control_id | source/revision | target/account | authority | desired | observed | observed_at | exact_head | positive_test | negative_test | evidence | gap | rollback | decision`

A ausência de permissão para provider-write exige `TOKEN_VAZIO_TOOL_CAPABILITY + HOLD` e tarefa explícita ao administrador — não simular sucesso.

**Referências oficiais:** [rulesets](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-rulesets/creating-rulesets-for-a-repository), [GitHub Secrets](https://docs.github.com/en/actions/security-for-github-actions/security-guides/using-secrets-in-github-actions).
