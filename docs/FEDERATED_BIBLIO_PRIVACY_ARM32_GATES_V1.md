# SETUP Ω — biblioteca federada, dupla entrada, governança e Android ARM32 (V1)

**Tipo:** plano operacional auditável; **estado:** `DRAFT / NÃO PROMOVER`; **data:** 2026-10-14; **claim_allowed=false**.

## Origem / anterioridade / delimitação

A camada biblioteconômica já pertence ao [Mapa](https://github.com/rafaelmeloreisnovo/Mapa/tree/main/biblioteconomia) e ao seu documento `23_NOVOEXPORT_BIBLIOTECA_SEMANTICA.md`; o corpus e os índices privados pertencem ao Google Drive NOVOexport; os produtores preservam a autoridade sobre o próprio código. Não criar uma biblioteca paralela neste SETUP nem inserir fragmentos privados nos commits públicos. A extensão atual do Mapa é o sucessor `24_BIBLIOFLOW_FEDERATED_51SHARDS_SYMBOLS_ARM32_V1.md`, com contrato `data/biblioteconomia/federated_biblio_contract.v1.json` (em branch PR separado, ainda sem promoção).

O SETUP mantém `config/setup-catalog.v1.json`, `config/gap-register.v1.json`, `tools/validate_setup.py` e `docs/GOOGLE_DRIVE.md` como autoridade pré-existente de PLAN_ONLY. Este sucessor é complementar; não liga provider, não concede escopo nem substitui os registros existentes.

## Provedores e quatro domínios de acesso

1. **GitHub público:** código, contratos, CI e gates aplicáveis somente a objetos públicos; não publicar IDs/trechos/metadados privados.
2. **GitHub privado:** código e evidência privada dos produtores com token/instalação restritos ao dono; nada de credencial em documento ou APK.
3. **Google Drive público:** somente objetos revisados e explicitamente divulgados; existência de link não prova divulgação aprovada.
4. **Google Drive privado:** NOVOexport, índices pessoais, shards, ledger/receipts com custódia; `account_alias,provider_id,revision,access,sha256_scope,observed_at` somente em superfície privada.
5. **alphaXiv:** papers e referências externas podem ser públicos; pastas, notas e highlights seguem `sharing_status` observado (padrão não publicar). Gravar no Drive apenas os metadados autorizados necessários e o resultado do gate.
6. **Celular Android ARM32 / APK:** frontend e adaptadores sob permissões Android fazem OAuth, TLS, SAF/MediaStore, rede e armazenamento. Núcleo puro L0 calcula rotas, digest/CRC e estado de parser quando demonstrado no SHA do produtor; não usar libc-free/zero-dependencies como alegação para a camada host. `BUILD_PASS` não comprova instalação física ARM32, nem comportamento sob desligamento de rede/memória.

A expressão `2⅓+2⅓1⅓` entra como notação literal `TOKEN_VAZIO_SEMANTIC` até gramática/escala serem definidas: não converter em quotas operacionais ou permissões automaticamente.

## Matriz de gates e prioridades

`P0` vazamento, mutation sem autoridade, commit público de dados privados, secret no APK, falsa declaração de prova → bloquear e preservar recibos.
`P1` divergência de hash/versão, perda de custody, branch protection/CodeQL/review/CI sem enforcement, artefato APK sem verificação exata → bloquear promoção.
`P2` conflitos semânticos, alias Unicode, temporalidade de conversa/mensagem, lacuna de identidade no join 51 shards/GitHub → sucessor tipado, não reescrever fontes.
`P3` novos pares bibliográficos, interoperabilidade, consulta acadêmica → explorar sem ampliar claim.

Fluxo: `INTENT → CURRENT_STATE → μREAD → SOURCE_MIN → AUTHORITY → ACCESS → ACT → EVIDENCE → EXACT_HEAD → INDEPENDENT_REVIEW → RECEIPT → μWRITE → R3`. Se qualquer gate bloqueante não passa: `HOLD` e a razão verificável; não dar merge.

## Dupla entrada e recibos

`ENTRY_A_SOURCE = source_provider + account_alias + object_id + revision/commit + digest_scope + source_timestamp + observed_timestamp + access_class`.

`ENTRY_B_EVENT = action + actor_authority + typed_relation + predecessor + successor + result + evidence_ref + gate_status + priority + rollback_pointer + next_probe`.

Persistir A/B no Drive privado com revisões append-only e readback. Apenas catálogos públicos sem links privados entram no GitHub. Uma IA deve ser capaz de responder onde está o original, quem autorizou, o que foi realmente feito, qual a prova, qual o estado e como voltar sem apagar histórico.

## Integração com as PRs atuais e dependências

As PRs #7, #8 e #9 do SETUP são DRAFT/NÃO PROMOVER. #7/#8 tratam do núcleo/controle; #9 trata do contrato Android↔Drive; esta PR apenas explicita requisitos federados e gates. Nenhuma outra PR fecha automaticamente essas lacunas. Falta provar na CI/HEAD exato e num APK ARM32 real: parsing adversarial, sem texto bruto nos logs, prova sem segredos, exibição legível, offline read-only, retentativa idempotente, rollback de índice derivado, restore de receipt, controle de permissão Android. Separar **receipt CORE**, **receipt BUILD/APK** e **receipt DEVICE**.

Para o roteamento por produtores (Rafaelia_Private, Gaia, RafaelIA, RafPolimata, RafGitTools, Mapa e outros), preservar a custódia de nomes/refs privados somente nas fichas autorizadas. Publicar em SETUP apenas os nomes que já são públicos, e sem converter ausência de acesso em inexistência de produto.

## Falsificadores e decisão

A documentação não prova que uma API ou integração foi executada. Rejeitar claims se: provider guard está OFF, CI pertence a SHA antigo, autorização não foi conferida, dispositivo não executou, pasta alphaXiv tem `not_shared` mas seu conteúdo foi exposto, fonte privada foi copiada para GitHub, cadeia A/B tem identidade inconsistente, ou enumeração do Drive foi parcial. `TOKEN_VAZIO` é estado conhecido de desconhecimento; não é zero, resultado negativo, nor `PASS`.

Reversão: fechar a PR, manter o estado da `main`; se incorporada posteriormente, revert por commit escopado com recibo sucessor. Não excluir raw, mover, sobrescrever Drive nem trocar permissões sem autorização específica.

## R3
`F_ok`: provedor/autoridade/biblioteca existentes identificados, nova matriz documentada.
`F_gap`: enforcement real, readback de Android/Drive, revisão humana independente, esquema da expressão 2⅓ e execução física são gates abertos.
`F_next`: validar `config/biblio-provider-gates.v1.json`, testes locais e CI do head; colher permissões/provider/Android sem expor segredos; deixar PR draft até conclusão.
