# Contrato humano + IA
Navegue: README → guia de iniciante ou técnico → catálogo → produtor. Antes de qualquer mutação externa use START HERE → CURRENT_STATE → 1–3 fontes mínimas → autoridade → evidência.
- Este SETUP **não** concede autoridade para aprovar PR, ativar rulesets, criar/ler Secrets, executar outros repos ou acessar Drive.
- Não registrar segredos, PATs, tokens OAuth, conteúdo pessoal, chave, certificado privado ou screenshots de segredos em arquivos, issues ou receipts.
- Secret **nomeado** ≠ presente; documento ≠ execução; CI local ≠ enforcement do provider.
- Sem merge automático, force push, exclusão, contorno de segurança, self-review ou licença inventada.
- Falta SOURCE, AUTHORITY, EXECUTION_TARGET ou EVIDENCE_RULE → `ROUTE_STATE=BLOCKED`; `TOKEN_VAZIO`.
- Alterações de código somente na branch/PR com teste de falsificação e rollback. Produtor governa execução.
- `python3 tools/validate_setup.py && python3 -m unittest discover -s tests -v`.
- Após delta comprovado: μWRITE no Drive como sucessor append-only e HOTSTATE se estado efetivo mudar. Nunca editar START HERE como diário.
