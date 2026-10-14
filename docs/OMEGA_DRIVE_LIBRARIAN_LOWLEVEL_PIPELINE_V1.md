# Ω Bibliotecário Drive — arquitetura low-level verificável, sem falsa execução

**Status:** `L0_IMPLEMENTED_UNTESTED` até execução da CI no SHA atual. Adaptador Android/Drive: `SPEC_ONLY / NOT_RUN`. Modelo append-only, autorização do titular e `SOURCE ≠ ARTIFACT ≠ EXECUTION ≠ EVIDENCE ≠ CLAIM`.

## Intenção, sem ficção de hardware

Usar um Android como *processador de blocos temporários*, com corpus/índices/receipts persistidos **no Google Drive autorizado**, governança no START HERE/HOTSTATE/LEDGER, código e CI no repositório produtor. O SETUP especifica e verifica contratos; **não se apropria da implementação dos demais repositórios** e não concede permissão OAuth.

```text
USER TRIGGER / ANDROID OS
       │
Drive adapter [OAuth + TLS + revisions + ACL, hosted]
       │ read exact 4KiB windows; streaming no raw logs
       ▼
L0 C freestanding [tri-state evidence gate + verified feature route]
       │ source bytes / hashes [algorithms from existing PRODUCER]
       ▼
Typed index projection [L/O/T/C + relation: verified / candidate / unknown]
       │
Drive adapter [private immutable staging, checksum/readback]
       │
append-only μRECEIPT [source → bytes → parser → transform → artefact]
       │
promote pointer only after required authority + negative-gate evidence
```

## Roteamento de capacidade — o que existe e o que não existe

| Capacidade | Produtor localizado | Situação operacional |
|---|---|---|
| CRC-32 IEEE software | `RafPolimata/tools/rafbbs/rafbbs_crc32_core.h` | Implementação própria sem runtime no núcleo, polinômio refletido `0xEDB88320`; usar sem reescrever |
| CRC-32 ARM intrinsics | `RafPolimata/Apkc/fmt_zip.h` | Possível quando `__ARM_FEATURE_CRC32`, compilador+CPU suportam; validar equivalência com vetor e binário exato |
| NUMA | `RafPolimata/freestanding/include/raf_fs_numa_plan.h` | Planejador consultivo com topologia entregue; **não prova memória física NUMA em Android** |
| NEON / SIMD | `GaiaPhiRafcode/raf_t7_neon_batch4.c`; `RafPolimata` com código NEON | Habilitar somente após ABI/auxv detectado e testes por modelo; processamento de 4KiB não garante aceleração |
| ZIPRAF / TTL / paridades | `Mapa/docs/ZIPRAF_CONTENT_PAGE_EXECUTION_GRAPH_V1.md` | Separação STORE vs DEFLATE, TTL epoch e observação ECC. Recuperação física de 25–45% **não demonstrada** |
| Bit Ghost | `Rafaelia_Private/docs/library_audit/2026-09-18/RAFAELIA_LIBRARY_AUDIT_RECEIPT.md` | `bitghosth.c` não localizado pela auditoria de 18/09; hoje apenas referências textuais, `TOKEN_VAZIO_SOURCE` |
| GPU | Vectras/Android como candidatos | Driver, ABI, shader/SPIR-V, memória transferida, ganho e consumo não medidos para esta tarefa. **Não ligado por padrão** |
| NIO / ART / DEX / Java | Android host adapter | Necessariamente runtime hospedado, separado do L0, com buffers controlados e JNI explícito |
| octa-core, L1/L2/L3 | Hardware/Android | Scheduler e caches geridos pelo SO/CPU; escolher tamanho de buffer, evitar cópias e medir cache misses quando possível. **Não controlar diretamente L1/L2/L3 pela aplicação** |

**Cuidado com grandezas.** Dois bytes contêm 16 bits, mas isso não demonstra 16 execuções simultâneas; para um bloco de 32 bytes (=256 bits), NEON Q 128-bit pode usar **duas cargas vetoriais** quando a operação e alinhamento permitirem. No AArch64 o conjunto de 32 vetores de 128 bits totaliza **4.096 bits = 512 bytes** de registradores; um buffer de **4.096 bytes** é memória distinta. O barramento, microarquitetura e mobilidade cache↔núcleo exigem prova medida.

**Sem sobreprometer:** `branchless` na operação de seleção não elimina verificações de autorização; `sem nomear variável` não deve prejudicar reconstruibilidade e testes. `sem overhead` é impossível por definição com TLS, rede, armazenamento, controle de versão e verificação. A otimização objetiva é minimizar cópias, alocações, viagens de rede e desvios desnecessários **com medições**.

## L0 compilável — o que decide

`incubator/omega_librarian_l0/omega_gate.c` é um controlador mínimo de evidência e escolha de rota, **sem I/O, libc, heap, syscalls, chamadas Android, globals mutáveis ou dependências de tempo**. Os nomes das variáveis pertencem ao código-fonte para auditar, não indicam alocação dinâmica.

Entradas tipadas: `source_identified`, `read_authority`, `source_sha256_verified`, `crc32_verified`, `revision/epoch`, `now_tick / expires_tick`, `destination_authorized`, `redaction_verified`, `write_receipt_verified`, `readback_verified`, `provider_gate_verified`, `erasure_present/recovery_verified`. Estados: `UNKNOWN=0`, `YES=1`, `NO=2`; **não confundir UNKNOWN com negação, nem com zero numérico de conteúdo**.

Saída `ALLOW / DENY / HOLD`, razão e rota `SCALAR` ou `NEON`. A função não executa SIMD nem lê arquivos. `attested_features` é testemunho **fornecido pelo chamador**, não uma prova independente de CPU, e por isso qualquer uso produtivo requer validação fora do núcleo. Na ausência de atestado, segue escalar. CRC ARM é uma opção de algoritmo já produzido pelo RafPolimata, **CRC não substitui SHA-256 de identidade**.

`READ` pode ocorrer antes do hash completo, com ACL e revisão conhecida; `INDEX` exige SHA-256 do material recebido. `WRITE` exige autorização do destino e ausência comprovada de corpo sensível indevidamente exportado; `PROMOTE` exige recibo de escrita, readback e política de provedor. Sem esses gates, fica HOLD ou DENY. *ALLOW significa que o conjunto declarado de pré-condições passou, não que a operação aconteceu.*

## Pipeline de gravação: contratos não implementados

Para cada lote:
1. Resolver `account_alias + file_id + revision + ACL + source_head`. `404` não implica exclusão. Não presumir permissões entre contas.
2. Ler e validar janelas fixas e comprimento exato com retomada por revisão. Se ETag/revisão mudou, descartar derivado antes de misturar versões.
3. Calcular digest SHA-256 dos bytes recebidos, CRC32 opcional para erro acidental, parser format-specific do produtor. Emitir falhas tipadas e hash por origem. Nenhum raw em logs.
4. Extrair somente índices mínimos e relações tipadas: direta, predecessora, interdomínio candidata, ortogonal, transversal, proveniência, evidência, contradição e `UNKNOWN_RELATION`. Correlação não vira causa.
5. Gravar nova versão **imutável e privada** do índice no Drive com idempotency key por `source_revision + schema + operation`. Sem substituir a fonte.
6. Fazer readback de bytes/revisão, conferir digests e ACL. Sem readback, `IMPLEMENTED_UNTESTED`.
7. Anexar `μID|timestamp|source|parent|kind|delta|routes|evidence|gap|next|hash` ao Ledger (append-only); atualizar `HOTSTATE` somente quando estado mudou.
8. Atualizar ponteiro do índice ativo sob revisão condicional; em conflito, não fazer lost update. Recuperação: apontar para snapshot anterior **sem apagar evidência**.

Drive não garante transação atômica distribuída entre arquivos; é necessário *staging + confirmação + ponteiro last*, idempotência, readback e rollback por referência. Os tokens OAuth nunca vão para GitHub, logs ou receipts. A conectividade e o sistema de permissões são **fronteira hospedada**, não freestanding.

## Bits perdidos, Huffman, ECC e Bit Ghost

Huffman/DEFLATE são compactadores **sem perda em funcionamento correto**; remover arbitrariamente 25–45% de seus bits normalmente torna a decodificação impossível. Uma reconstrução de apagamentos de 25–45% só pode ser reivindicada quando:
- existe redundância/código corretor com taxa e posições de apagamento adequadas;
- os testes exercitam precisamente 25, 35 e 45%, falhas em bursts, corrupção desconhecida e permutação;
- o payload reconstruído confere com SHA-256 autenticado do original.

CRC-32 detecta muitos erros acidentais, mas não recupera conteúdo. Estados `BIT_FLIP`, `KNOWN_ERASURE`, `OMISSION`, `PERMUTATION`, `UNKNOWN_SOURCE`, `TOKEN_VAZIO`, `ZERO` permanecem separados. A simulação não prova recuperação física nem retenção no Drive.

## Gate, evidência e rollback

- `bash tools/check_omega_l0.sh`: compilar L0 com `-ffreestanding -nostdinc -fno-builtin`, testar `nm -u` vazio e negativos C hospedados. Nenhum runtime Android ou login Drive é executado por essa CI.
- `python3 tools/validate_setup.py` e testes atuais preservados.
- `config/omega_drive_librarian_plan.v1.json`: formato legível por humanos/IA, etapas, autoridade, exceções e status sem ficção.
- O dono da execução Android/Termux deverá fornecer adapter real em seu próprio repositório, com permissão explícita, testes de carga, bateria/termia, I/O, latência e rollback.

**R3** = <F_ok: núcleo candidato e contrato, F_gap: teste exato CI + adapter autenticação/readback, GPU/NUMA/NEON hardware e recuperação de perdas, F_next: teste L0 → revisão independente → implementar adapter mínimo no produtor autorizado → receipt físico e Drive>. Nenhuma promoção ou merge automático.

## PR #7 → PR #8: dependência tipada, sem duplicar processamento

A leitura posterior identificou o [PR #7](https://github.com/rafaelmeloreisnovo/SETUP_RAFAELIA/pull/7) no mesmo SETUP, baseado no mesmo HEAD anterior, com `raf_bib_begin/raf_bib_feed/raf_bib_stage/raf_bib_ack`, CRC32 IEEE e buffer de 4096 bytes. Esse **é o produtor dos bytes e da prova de stage/ack**. O [PR #8](https://github.com/rafaelmeloreisnovo/SETUP_RAFAELIA/pull/8) tem base explicitamente apontada para o branch do PR #7 e fornece apenas o **gate de política `omega_gate_decide`**. Não implementar outro CRC, outro scanner de shard, outra fila de threads nem substituir o parser. A separação de responsabilidades é:

```text
adapter (host; account/revision/permissions)
  -> omega_gate_decide READ (pré-condições declaradas)
  -> PR7 raf_bib_begin/feed/stage (integridade e sequência)
  -> omega_gate_decide INDEX/WRITE (SHA/ACL/redaction/epoch)
  -> adapter Drive write + readback real
  -> PR7 raf_bib_ack (contrato do recibo)
  -> omega_gate_decide PROMOTE (readback e provider)
```

**Limite importante:** a interface PR7→PR8 acima está especificada, mas **ainda não foi compilada/testada num mesmo HEAD integrado**. O sucesso das duas CIs separadas não comprova funcionamento combinado. O código #7 declara SPDX MIT; o código #8 usa `LicenseRef-RAFCODE-Research-Commercial-0.1`; essa incompatibilidade potencial de distribuição/integração precisa de **decisão expressa do titular antes de qualquer merge**. Requisitos jurídicos/autorais não são resolvidos automaticamente pelo plano. Os dois PRs permanecem draft e não promovidos.
