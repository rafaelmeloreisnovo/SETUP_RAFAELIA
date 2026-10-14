# RAFAELIA Ω — Bibliotecário Drive L0 / Pipeline de custódia V1

**Estado:** IMPLEMENTED_UNTESTED até compilar no HEAD exato; operação com Drive = NOT_IMPLEMENTED; nenhum treinamento, sincronização autônoma, APK, provider write ou permissões novas. Fonte de norma: START HERE V2.2 → HOTSTATE → Ledger → SETUP. Este repositório continua PLAN_ONLY.

## O que o usuário pretende

Android como **processador transitório**, Google Drive como **fonte e destino documental**. Indexar, classificar por candidatos, deduplicar metadados e orientar rotas L/O/T/C em vez de espelhar arquivos privados ou treinar um modelo. GPU, NEON SIMD, NUMA, caches, CRC em hardware, NIO, ELF/DEX/ART e máquina de estados entram como capacidades optativas da plataforma, não como afirmações de execução automática.

### Roteamento humano e de IA, compacto

`START HERE V2.2 → CURRENT_STATE → µREAD 1–3 roots → SOURCE_MIN → DEDUP/CLASSIFY → STAGE INDEX → DRIVE WRITE → DRIVE READBACK → RECEIPT → µWRITE → R3`.

**Eixos:** `L` longitudinal (linhagem, revisões, instantes), `O` ortogonal (classificações independentes), `T` transversal (relações entre domínios), `C` contextual (intenção e sentido local), `P` proveniência, `R` relacional, `I` indexical, `E` evidencial, `A` adaptativo, sempre com estado `OBSERVED / DECLARED / HYPOTHESIS / NOT_RUN / TOKEN_VAZIO`. Arestas verbais, não verbais, desconhecidas ou extra-relacionais são rotas **candidatas**, não causais nem verificadas sem prova.

### Fronteiras e ordem transacional

1. **Hosted Drive/Android boundary (NÃO implementado neste PR):** OAuth pela conta/pasta autorizada, mínimo privilégio, TLS, identidade `account + file_id + revision`; ler blocos para buffer temporário; produzir SHA-256 de bytes exatos, testar SHA de origem contra manifesto autoritativo; carregar no núcleo somente a atestação de bytes. Esta camada possui dependências nativas/Java/Kotlin/NIO/NDK/ART/DEX e **não é freestanding**.
2. **L0 puro (implementado):** `raf_bib_begin → raf_bib_feed(≤4096 bytes, offset exato) → raf_bib_stage → raf_bib_ack`. CRC32/IEEE `0xEDB88320` portátil e estado binário serializável em estruturas do chamador, sem libc, libm, heap, syscalls, acesso a disco, conexão, objeto Java, C++ ou estado global. Ciclos do processador e eficiência ficam a medir; sem afirmação `zero overhead`.
3. **Adapter Drive write (NÃO implementado):** escrever **índice derivado** e recibo append-only em arquivo específico autorizado, consultar versão e SHA-256 do recibo por nova leitura. Na fase `stage`, L0 compara o SHA-256 recebido da fonte contra o valor previamente esperado e registra o SHA-256 do recibo; somente `ack` com SHA-256 do recibo relido e revisão confirmada muda para `ACKED`. L0 **não calcula SHA-256**: o adaptador deve fornecer o digest verdadeiro. Não chamar atestação do chamador de prova independente nem tratar duas verificações da mesma origem como revisores independentes.
4. **Result gate:** `UNKNOWN` durante leitura/staged; `ALLOW` somente quando `ACKED` após readback alegado pela fronteira de plataforma; `DENY` para estado inválido, erro explícito ou política do host. Não usar `ALLOW` como autorização de merge ou publicação: provider GitHub rulesets e revisão independente são outras condições.
5. **Receipts privados:** `source_id, source_revision, source_SHA256, bytes, CRC32, block_count, index_schema, stage_SHA256, readback_file_id, readback_revision, readback_SHA256, valid_gates, unknown_gaps, previous_receipt_pointer`. Cuidado com IDs derivados em repositórios públicos e com colisões; SHA-256 não é autorização nem criptografia.

**Atomicidade:** Google Drive não fornece transação ACID global entre arquivos. Use escrita de objeto novo/versão e recibo idempotente; se a gravação ocorrer sem readback, marque `STAGED_UNKNOWN`, reconsulte pelo mesmo ID em vez de gravar outra vez. Não declarar rollback físico como concluído sem recibo da compensação. TTL é tempo de validade declarado de cache ou fila, não contador físico/timer garantido pela CPU; prazo real e alarme são responsabilidade da plataforma.

## Evidências pré-existentes reutilizadas sem copiar implementações

- `RafPolimata/tools/rafbbs/rafbbs_crc32_core.h`: CRC-32/ZIP freestanding de referência; o núcleo V1 aqui usa o mesmo polinômio e teste `123456789 → CBF43926`. **CRC não é SHA-256, não autentica custódia e não corrige perda de dados.**
- `RafPolimata/freestanding/include/raf_fs_numa_plan.h`: escolha consultiva com topologia fornecida; não prova presença de NUMA físico nem afinidade Android.
- `RafGitTools/COMPILER/README_RAFAELIA_ROOT_OPTIMIZER.md`: branchless no subconjunto comprovado, não política geral nem execução de todo o sistema sem ramificações.
- `GaiaPhiRafcode/raf_t7_neon_batch4.c`: 4 lanes de 32 bits por registrador NEON Q de 128 bits; benchmark tem funções hosted, portanto não reproduz automaticamente Android GPU ou processamento de 51 shards.
- `Vectras-VM-Android/engine/rmr/include/bitomega.h`: estado BitOmega (10 estados e VOID) licenciado **GPL-2.0-only**; referência conceitual, código não copiado para SETUP MIT. `bitghosth.c` exato ainda TOKEN_VAZIO na auditoria de 2026-09-18 de Rafaelia_Private.
- `SETUP_RAFAELIA/docs/GOOGLE_DRIVE.md`: OAuth, identidade, revisões, privacidade e recibos tipados. Docs e CATÁLOGO existentes continuam fonte de política, não são reescritos.

## Hardware e unidades — hipóteses, não medições

| Mecanismo | Uso condicional | Gate |
|---|---|---|
| SIMD NEON | Processamento vetorial de bytes alinhados quando benéfico | Detectar extensão ISA por dispositivo, comparar saída scalar bit a bit e medir custo de preparação |
| CRC32 em hardware | Caminho opcional após feature probe | Verificar **polinômio IEEE vs CRC32C**; instrução ARM CRC32 não é automaticamente compatível com outra família CRC |
| Octa-core / scheduling | OS agenda threads e migra núcleos conforme energia/carga | Não afirmar todos os oito núcleos trabalhando nem cache fixo |
| L1/L2/L3 | Cache de CPU é majoritariamente governado pelo hardware | Não acessar cache físico como buffer de usuário; afinidade e cache hints são dependentes da plataforma |
| NUMA | Planejador aplicável com topologia real fornecida | Em celulares pode não haver topologia NUMA exposta; `TOPOLOGY_UNKNOWN` é válido |
| GPU | Lotes grandes e computação com driver autorizado | Driver/API/sincronização e custos de transferência; não prometer `freestanding GPU` sem boundary documentada |
| NIO/Java/ART/DEX | Arquivos/streams no adaptador Android | Framework hospedado, nunca dizer `pure L0` |
| 4 KiB | Buffer de leitura V1, máximo de 4096 **bytes** | Medir desempenho, não confundir com 4096 **bits** do banco AArch64 |
| `16 × 2 bytes` | 32 bytes = 256 bits = dois vetores NEON de 128 bits, se corretamente mapeados | Largura efetiva de barramento/cache NÃO comprovada por essa expressão |
| ZIP/Huffman/BitGhost | Compressão lossless com dicionário e metadados suficientes | Ausência de 25–45% dos bits de fonte **não é reversível arbitrariamente**; exige redundância/código corretor/outra cópia e prova por hash |

## Falsificadores de V1

O teste `tests/test_raf_drive_librarian_l0.c` precisa rejeitar offset fora de ordem, buffer superior a 4096, SHA divergente, fonte truncada, revision readback ausente e ACK duplicado sem alterar contexto; deve confirmar CRC32 padrão e paridade `1×4096 == 16×256`, assim como `TOKEN_VAZIO != 0`. CI compila `-ffreestanding -nostdinc -fno-builtin` e exige `nm -u` sem símbolos externos **do objeto**. Nenhum deles testa OAuth, Drive, Android, GPU, CPU cache, NUMA, NEON real, NIO físico, code scan, SHA-256 pelo núcleo nem proteção de merge.

### Regra de promoção

`SOURCE ≠ ARTIFACT ≠ EXECUTION ≠ EVIDENCE ≠ CLAIM`.
`IMPLEMENTED_UNTESTED ≠ PASS`.
`TOKEN_VAZIO ≠ 0`.

**NÃO PROMOVER** até testes no SHA exato, revisão de licença/autoria, readback real do Drive, prova do provider enforcement e falsificador de retry sem duplicar índices. A existência do código não ativa sincronização nem autoriza escrever mensagens privadas no GitHub.

R3 = `<F_ok: contrato freestanding e testes de fonte, F_gap: adaptador autenticado/driver/hardware/recibos físicos/provider, F_next: CI exact HEAD; em seguida conectar um único arquivo consentido e provar readback SHA256 e idempotência>`.
