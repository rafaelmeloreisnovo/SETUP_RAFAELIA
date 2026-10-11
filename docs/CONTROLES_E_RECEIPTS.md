# Casa de Governança: papéis, direitos, gates e provas

| Papel | Autoridade típica | Restrição |
| --- | --- | --- |
| Iniciante | ler, escolher objetivos, relatar falhas | não divulgar credenciais nem afirmar PASS |
| Mantenedor | PR e testes no produtor | não aprovar o próprio PR como independente |
| Admin do provedor | alterar regras conforme mandato | precisa de readback, rollback e controles reais |
| Revisor independente | revisar diff/evidências e decidir | não aceitar bot/owner como prova de independência |
| Custodiante Drive | validar arquivo, conta e permissão | não transferir conteúdo privado para GitHub público |
| Agente | observar, classificar lacunas, preparar plano | não se autopromover nem inventar fatos |

## Seis passos do gate
1. **SOURCE**: licença, autoria, corpus/versionamento do produtor e legitimidade.
2. **AUTHORITY**: operador e permissão para a ação exata.
3. **SETTING**: aplicar no provedor certo (manual / aprovado).
4. **EXECUTION**: teste terminal no HEAD exato, sem confundir lint com runtime.
5. **EVIDENCE**: readback positivo e falsificador negativo, sem credenciais visíveis.
6. **DECISION**: aprovação independente e recibo append-only; rollback/revisão se divergente.

**Falha segura:** `TOKEN_VAZIO` não é zero; `DECLARED` não é `APPLIED`; `IMPLEMENTED_UNTESTED` não é `PASS_SCOPED`; `PASS_SCOPED` não autoriza `GLOBAL_PRODUCTION`.

## Receipt μWRITE
`μID | ts | source/ref | parent | kind | delta | route | exact_head/revision | authority | execution | evidence | gap | next | rollback`.

Registrar diferença material no ledger do Drive como **sucessor**, sem apagar histórico. Atualizar HOTSTATE só quando realmente mudar a rota/estado. Manter START HERE como roteador, não depósito.

## Direitos, zero trust e risco
- Não copiar código upstream sem licença autorizativa e preservação de avisos.
- Não usar dados pessoais sem autorização, finalidade e minimização.
- Identidade em GitHub **não** é autoridade no Drive, e vice-versa.
- Não usar permissões amplas, bypass permanentes ou PAT longo sem justificativa.
- Conferir incidentes e reverter por mudança mínima aprovada, nunca por exclusão indiscriminada.
