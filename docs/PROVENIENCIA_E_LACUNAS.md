# Proveniência Ω V2: de lacuna a prova

**Escopo:** observação de 11/10/2026, não configuração externa. O arquivo `config/gap-register.v1.json` associa **18 controles** a estados, causas, fontes e falsificadores mínimos.

## Evidência que reduziu incerteza
- [SETUP main](https://github.com/rafaelmeloreisnovo/SETUP_RAFAELIA): HEAD `67a72a6cf6b9c8fbbfdbdcf25b44b74ff57898ad`; `protected=false`, checks `off`, zero rulesets observados.
- [SETUP PR #1](https://github.com/rafaelmeloreisnovo/SETUP_RAFAELIA/pull/1): `merged=true` no SHA acima, sem revisão submetida na resposta do endpoint. Merge não prova revisão independente.
- [Mapa main](https://github.com/rafaelmeloreisnovo/Mapa): HEAD `de3aa28e203a84f291a61453e0e95f75f911650a`; `protected=false`, ruleset #21909304 desativado, quatro exceções `always` requerem apuração de identidade.
- [Mapa PR #750](https://github.com/rafaelmeloreisnovo/Mapa/pull/750): aberto, não incorporado; resposta de reviews vazia.

Esses fatos derivam de leitura nativa do conector GitHub e estão vinculados a URLs públicas. São fotografia de um momento, não garantia de estado futuro. Bytes HTTP originais não foram observados; não reivindicar hash wire nem certificação de segurança.

## Três tipos de lacuna
- `OPEN_OBSERVED`: há observação concreta de um bloqueio, como proteção desligada.
- `PARTIAL_OBSERVED`: parte conhecida, mas a identidade, direito ou prova final ainda falta.
- `TOKEN_VAZIO`: desconhecido; não implica ausência, sucesso ou falha.

## Fechamento
`INTENT → FONTE → AUTORIDADE → EXPERIMENTO MÍNIMO → FALSIFICADOR NEGATIVO → READBACK → REVISÃO → RECEIPT → DECISÃO`.

**Exemplo:** ruleset escrito mas desligado não basta. Administrador autorizado ativa critérios apropriados, lê o estado efetivo e comprova que um PR benigno sem aprovação é bloqueado **sem realizar merge**, preservando `main`. CodeScan precisa de análise real e SARIF não vazio; `GITHUB_TOKEN` e segredos não são transferidos a este SETUP.

**Direitos e segurança:** `SOURCE ≠ ARTIFACT ≠ EXECUTION ≠ EVIDENCE ≠ CLAIM`; `TOKEN_VAZIO ≠ 0`; `IMPLEMENTED_UNTESTED ≠ PASS`. A aprovação humana não pode ser inventada pela IA.

## Resultado deste PR
O validator apenas garante catálogo ↔ registro de lacunas e bloqueia declarações de promoção local. `PASS_SCOPED` não ativa rulesets nem cria credenciais. Não usar a planilha/README para expor valores de segredos. Rollback por fechamento do PR ou revert escopado autorizado; preservar histórico.
