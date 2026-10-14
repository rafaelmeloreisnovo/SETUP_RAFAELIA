# RAFAELIA Ω — Android como processador; Drive como biblioteca persistente (contrato V1)

State: **PLAN_ONLY / DRAFT / NO_PROMOTION**. Owner: `SETUP_RAFAELIA` only for integration policy and routing. No OAuth, GitHub settings, provider mutation, source deletion, APK, background service, GPU execution, or Android physical benchmarks implemented by this document.

## 0. Proveniência e autoridade

Read order: [START HERE Ω V2.2](https://docs.google.com/document/d/1T4OIN2RC1I-KFsdaQTCqieeZUGYBXissWrdrZMG0DN8/edit) → [CURRENT_STATE Ω V4.5](https://docs.google.com/document/d/1d0J5SkF2S2emBq6jLuTkhYVaSOQiWazJWyRCTMJ-iRI/edit) → minimal producer source → authority → evidence → append-only receipt.
Invariants: `SOURCE != ARTIFACT != EXECUTION != EVIDENCE != CLAIM`; `TOKEN_VAZIO != 0`; `IMPLEMENTED_UNTESTED != PASS`.
Do not copy private Drive payload into GitHub. A Drive pointer is not byte identity or proof of user authority. Current setup PLAN_ONLY cannot authorize Drive-wide traversal.

## 0A. Reconciliação de PRs existentes — esta proposta NÃO produz novo L0

A investigação do HOTSTATE e das PRs abertas encontrou **dois predecessores na mesma área**, que têm prioridade sobre este contrato documental:

- [SETUP PR #7](https://github.com/rafaelmeloreisnovo/SETUP_RAFAELIA/pull/7), head `29830006d045157c8cb009606877e825de2cc456`: `experimental/drive_librarian_l0/` com `BEGIN→FEED→STAGE→ACK`, CRC32 C incremental, até 4096 bytes e falsificadores. Runs 38512496106 + 38512495989 foram relatados como SUCCESS exact-head, apenas escopo objeto/host.
- [SETUP PR #8](https://github.com/rafaelmeloreisnovo/SETUP_RAFAELIA/pull/8): controlador de **política complementar** `incubator/omega_librarian_l0/`, `ALLOW/DENY/HOLD`, revisão/epoch/TTL/readback e escolha consultiva de caminho. Run 38512625550 SUCCESS objeto/host segundo receipt da PR; depende de interface/autoridade de PR #7.

**Decisão de deduplicação:** esta PR #9 é somente a ponte de arquitetura L1 Google Drive ↔ Android e especificação de indexação semântica/ATLAS. NÃO introduzir implementação concorrente de CRC, buffer, estado `BEGIN/FEED/STAGE/ACK`, autorização ou BitGhost. Atestação de L0 e interoperabilidade PR7→PR8 exigem revisão independente; PR7/PR8 permanecem DRAFT/HOLD até os gates próprios. Se um produtor consolidar o núcleo fora de SETUP, atualizar referência por successor, nunca fingir transferência de autoria.

O artefato da PR #9 é desenho e rotas; **não contém código de execução**. O `SOURCE` de BitGhost está precisamente no header Vectras GPL-3.0-only indicado abaixo. O que permanece `TOKEN_VAZIO` é a integração autorizada e testada, não o localizador do source.

## 0B. Successor de auditoria — compressão/reconstrução já materializadas (2026-10-14)

**Corrige a generalização anterior**: o `Huffman placeholder` em `llamaRafaelia/rafaelia-baremetal/zipraf/raf_zipraf.c` é exclusivo desse arquivo histórico e **não** significa que a compressão RAFAELIA não foi concluída em outros produtores. Antes de escrever motor próprio, escolher entre os artefatos reais:

- [ZIPRAF Ω G1–G11A checkpoint](https://github.com/rafaelmeloreisnovo/ZIPRAF_OMEGA_FULL/blob/ba1fe51f9d88cfbb6effaaac22df6d5ca77543e1/docs/ZIPRAF_G1_G11A_CHECKPOINT_2026-08-11.md): reports ZIP/STORE + SHA/CRC + byte-identical roundtrip, BITRAF V1 and ARM32 physical verifier. This is **packaging/integrity**, not proof of positive compression ratio.
- [Bitraf64 v1.4 RLE2](https://github.com/rafaelmeloreisnovo/ZIPRAF_OMEGA_FULL/blob/ba1fe51f9d88cfbb6effaaac22df6d5ca77543e1/bitraf64_rle_v2.py) + [container integration](https://github.com/rafaelmeloreisnovo/ZIPRAF_OMEGA_FULL/blob/ba1fe51f9d88cfbb6effaaac22df6d5ca77543e1/bitraf64_compressor_v14.py): escaped literal 0xFF, bounded decode and version-separated v1.3 legacy. The *legacy* v1.3 collision `FF 41 42` is a documented counterexample, not a current v1.4 failure. The v1.4 `hash_blake3` label remains simulated; no genuine BLAKE3 claim. The RLE2 raw 256-byte all-symbols example expands to 257 raw / 344 Base64 bytes before BRAF metadata; storage ratio is data-dependent.
- [Rafaelia_Private LZ4 C](https://github.com/rafaelmeloreisnovo/Rafaelia_Private/blob/d95a54295116b4d36a2a95c24be4c2ca4b9a7745/src/hd_cache/compress_lz4.c) + [phase-1 receipt](https://github.com/rafaelmeloreisnovo/Rafaelia_Private/blob/d95a54295116b4d36a2a95c24be4c2ca4b9a7745/docs/phase1/PHASE1_IMPLEMENTATION_RECEIPT.md): self-contained block compression and adversarial roundtrip locally verified; `malloc`/libc dependencies in this module mean not L0 freestanding as-is.
- [RIVER-7 [7,4,3] erasure reconstruction](https://github.com/rafaelmeloreisnovo/ChipQuantum/blob/825321b85efdd46bf411e0f6013b9df0fdacf020/experiments/river7_dual_transport/simulate_linear_erasure_743.py): independent GF(2) rank enumeration yields 7/7 one-erasure, 21/21 two-erasure, 28/35 three-erasure patterns. Therefore **28.6% symbol erasure is guaranteed**, while 42.9% is conditional, not arbitrary corrupted bits. Burst-placement success is simulation-scoped, not physical memory repair. Related model: [BITRAF loss-vector index](https://github.com/rafaelmeloreisnovo/Mapa/blob/3a38e9f6fc7dd04169d13c42a1f274a4530fdc31/indices/BITRAF_LOSS_VECTOR_MODEL_V1.md).

Risk boundaries: `STORE != COMPRESS`, `CRC != REPAIR`, `erasure_location_known != bit_flip_unknown`, `synthetic_rank_proof != physical_device_recovery`. The inherited Bitraf64 decompressor can return `data` with `verified=false`; hosted callers must enforce verified before accepting. Do not deduplicate producer implementations merely by algorithm family or copy incompatible licenses.

The prior `ZIPRAF prototype` table row is retained as a narrow per-file observation. **Status**: compression implemented in multiple branches, but a universal completion/ratio/physical-recovery claim remains gated by producer version, tested source, input class and exact-head receipts. PR9 owns only the Drive librarian adapter design, **not** these codecs.

## 0C. Auditoria de famílias/versões por SOURCE, não por palavras (successor 2026-10-14)

This section documents the explicit follow-up named-term list without creating a competing ontology. Canonical identifiers/aliases belong to [Mapa architecture family registry](https://github.com/rafaelmeloreisnovo/Mapa/blob/3a38e9f6fc7dd04169d13c42a1f274a4530fdc31/data/ontology/architectures/ARCHITECTURE_FAMILY_REGISTRY_V1.yaml) + [discovery rules](https://github.com/rafaelmeloreisnovo/Mapa/blob/3a38e9f6fc7dd04169d13c42a1f274a4530fdc31/data/ontology/architectures/ARCHITECTURE_DISCOVERY_RULES_V1.json). This Mapa seed states 144 source-bound conversations/179 title observations, **not an exhaustive chronology of all exports, Drive roots, Git history or forks**. Do not treat failed default-branch code search as evidence of nonexistence.

### Physical code routes observed

| User tokens / variants | Exact source route | Scoped classification |
| --- | --- | --- |
| `rmrcti`, `rmr/engine`, `rmr/core`, `rmr/baremetal` | [llamaRafaelia/rmrCti/INDEX.md](https://github.com/rafaelmeloreisnovo/llamaRafaelia/blob/b532b7678e4f86b1b5dccf5f54a6b310f9014b34/rmrCti/INDEX.md); [Vectras engine/rmr README](https://github.com/rafaelmeloreisnovo/Vectras-VM-Android/blob/6e4d99c8d90a3a1ddcb68933c693815d7c6e847b/engine/rmr/README.md); [unified kernel](https://github.com/rafaelmeloreisnovo/Vectras-VM-Android/blob/6e4d99c8d90a3a1ddcb68933c693815d7c6e847b/engine/rmr/src/rmr_unified_kernel.c); `rmr_baremetal_compat.c` | IMPLEMENTATION_SOURCE; RMRCTI raw CTI scanner/index/graph, RMR routing state, baremetal compatibility; the four verbal paths are not asserted to be literal identical directories |
| `rmr/bitomega`, `bitomega` | [C finite state implementation](https://github.com/rafaelmeloreisnovo/Vectras-VM-Android/blob/6e4d99c8d90a3a1ddcb68933c693815d7c6e847b/engine/rmr/src/bitomega.c) | IMPLEMENTED_SOURCE: 10 states with Q16.16 state/context; no bit compression claim |
| `btiraf` → candidate alias `BITRAF` | [Vectras bitraf.c](https://github.com/rafaelmeloreisnovo/Vectras-VM-Android/blob/6e4d99c8d90a3a1ddcb68933c693815d7c6e847b/engine/rmr/src/bitraf.c) | IMPLEMENTED_SOURCE: per-64-byte CRC32 table + XOR-reversible framing. Exact size `n + 32 + 4*ceil(n/64)`; **not byte-size reduction**. `btiraf` spelling remains QUERY_ALIAS_UNCONFIRMED |
| `rafaelia_b1..b14`, `fiber*` | [Termux rmr/Rrr](https://github.com/rafaelmeloreisnovo/termux-app-rafacodephi/tree/d56d18e57dc2a358d8fb215a0e8687fce18b8ee0/rmr/Rrr) contains `rafaelia_b1.S` through `rafaelia_b8.S`; `build_all.sh` invokes B1–B5 only; [GAIA FIBER-H](https://github.com/rafaelmeloreisnovo/GAIA_phi/blob/264a698756b81bb952fa2b0e4bf80b4d59720d35/fiber_kernel.c) and `fiber_hash.c`/avalanche/ ECC skeleton | B1–B8 SOURCE_OBSERVED; B9–B14 **not located in that exact directory**; historical/alternate repo/branch status TOKEN_VAZIO. FIBER hash vs ECC conceptual skeleton vs profile benchmark separate |
| `voy*`, `bgc color`, `gbc color`, `GBS3` | [RafPolimata Voynich barecore](https://github.com/rafaelmeloreisnovo/RafPolimata/blob/8d0fb29904b0bbd990b8fec08c57e5ab05db42dd/experiments/voynich_repro/c_baremetal/README.md); [llamaRafaelia gbs3_color.c](https://github.com/rafaelmeloreisnovo/llamaRafaelia/blob/b532b7678e4f86b1b5dccf5f54a6b310f9014b34/rmrCti/gbs3_color.c); [prior source-audit pointer](https://github.com/rafaelmeloreisnovo/CONVERSATIONS_CHUNKS_PRIVATE/blob/main/auditoria/vector_manifold_novoexport_v1/findings/2026-09-25_code_verification.md) | VOY synthetic barecore source, physical AArch64 unverified; GBC/GCB **oral alias** resolved to hosted C GBS3 stability trace/ΔP, not same as LayersBit |
| `omini` → candidate alias `omni`, `omni core` | `Rafaelia_Private/aRafaeilia/rafaelia_omni_core.c`, `Rafaelia_Private/src/python/raf_omni.py`, `GAIA_phi/rafaelia_omni_core_v107.py` | MULTIPLE distinct C/Python implementations; do not collapse legacy `v999` label, `v107` module and OMNI interface into one runtime without hashes, API/behavior diff and execution receipt |
| `hypercubo`, `hcpm`, `hypercerebro`, `fcea` | Mapa ARCH-FCEA, ARCH-HCPM, ARCH-FCEA-HYPERCUBE; `privadoFazendo/hcpm.py`, `Rafaelia_Private/new_data/nhypercubo_core.c`, `privadoFazendo/UNO_INTENCAO_PURA.sh` | MIXED: partial Python/C, scripts, documented cognitive family; HYPERCEREBRO also string in launcher, no implied independent L0 |
| `avalanche`, `soc`, `voo quantum`, `quantum` | `GAIA_phi/fiber_avalanche_montecarlo.c`; `Rafaelia_Private/aRafaeilia/rafaelia_matrix_soc.c`; `Rafaelia_Private/Anews/New/voo_quantico.sh` and `voo_json_quantico.py`; `RafaelIA_Solucoes_Clay/Rafaelianos/Formas voo` | RESEARCH/EXPERIMENT/EXECUTABLE_SCRIPT; names do not establish physical quantum effects |
| `rafaelianos`, `rafaeliana`, `fibo vetorial`, `fractal omega` | `Matem-tica-/src/verify_rafaeliana_prime_factor.py`; `CientiEspiritual/docs/Num_Rafaelianos.txt`; `ChipQuantum/src/geometry/sqrt3_geometry_matrix/relation_calculus.py`; `RafPolimata` Fibonacci geometry routes | FORMAL/NUMERICAL/RESEARCH. Mathematical source and exact hypothesis must be pinned separately; not a compression codec |
| `z0`, `zero absoluto`, `zeta`, `zetta`, `beta`, `gama` | `X0/Z0_KERNEL.sh`, `Rafaelia_Private/Ordernar/ra2/z0_universo.c`, Mapa ARCH-ZETA and unresolved Beta/Gama | MULTIPLE DISTINCT semantic families; `ZETTA` search surfaced SI storage prefix, **not proven alias** of `ZETA`; thermal 0 Kelvin != software state Z0 |
| `ratata`, `raia`, `∆lacuna`, `∆mind`, `godex`, `verbo vivo` | [Mapa registry](https://github.com/rafaelmeloreisnovo/Mapa/blob/3a38e9f6fc7dd04169d13c42a1f274a4530fdc31/data/ontology/architectures/ARCHITECTURE_FAMILY_REGISTRY_V1.yaml), `privadoFazendo/verbo_vivo_raia∆.sh`, `privadoFazendo/lacuna/void/lacuna_vision.py`, `privadoFazendo/.rafaelia/∆mind_align.sh`, `privadoFazendo/GODEX/core/godex_main.py` | RATATA alias of source-bound RARATA but other identities separate. Implementations/scripts mixed with manifesto/documentary occurrences |
| `protocolo 777`, `999`, `188`, `comando 0`, `cmd 0`, `metaverso` | `Rafaelia_Private/Anews/New/voo_quantico.sh` labelled V777; `X0/fcea_core/scripts/cvv188.sh`; `X0/rafaelIA.sh` Z0/CVV188/999; `X0/termux_backup_20250522_011520/RAFAELIA_CORE/scripts/interpreter_simbiotico.sh` command 999; Mapa CMD family unresolved | VERSION/PROTOCOL/COMMAND ambiguous; V777 script != proof of one canonical Protocol 777; 188 vs CVV188 candidate binding; CMD0 no verified programmatic command identity |
| `v50`, `v70`, `v78`, `v79`, `v80`, `v1000`, `v2000`, `v2001`, `maia`, `supreme omega fractal` | `privadoFazendo/RAFAELIA_V9999/src/core_v{50,70,78,79,80,1000,2000,2001}.sh`; `V79-1/docs/FRAMEWORK_COGNITIVO_VOYNICK_ABSOLUTO_V79_1.md`; Mapa ARCH-MAYA-LINEAGE | All numbered wrapper scripts exist; `core_v2001.sh` merely echoes, invokes `core_v2000.sh` and sleeps 0.5sec — do NOT infer engine evolution from filename. V79.1 separately has scaffold. Maya/Infinity claimed predecessor relations need source/hash proof; version label V1000 for scripts is distinct from proposed licensing V1000, and neither implies a universal successor |
| `deepGenerator`, `axc`, `institutorafael` as AI, `DMT`, `AxB` | No exact `deepGenerator` match in currently indexed default code search; `Rafaelia_Private/Anews/New/universal.py` has DMT geometry; `RafGitTools` AxB tensor Q8.8 mention; `Instituto Rafael` appears as institution/author label in source headers | DEEPGENERATOR/AXC/INSTITUTORafael_AI: TOKEN_VAZIO_SOURCE_BOUND; DMT/AxB ambiguous technical usages. A human/institution header does not establish an implemented AI process |

### Corrections / falsifiers before integration

1. **Never equate CODE NAME with implementation**: `bitraf_compress` in Vectras generates a larger framed XOR payload; Bitraf64 v1.4 RLE2, Rafaelia_Private LZ4, ZIPRAF STORE and RIVER7 parity are distinct operations. `CRC32 IEEE != CRC32C`.
2. `termux-app/rmr/Rrr/build_all.sh` only builds B1..B5 despite B6..B8 files. Add exact SHA build/run gates for B6..B8 on their producer before integrating. Do not claim physical GPU: [gpu_ema_step](https://github.com/rafaelmeloreisnovo/termux-app-rafacodephi/blob/d56d18e57dc2a358d8fb215a0e8687fce18b8ee0/rmr/Rrr/rafaelia_gpu_mid.c) falls back to CPU for OpenCL/Vulkan; `gpu_init` detecting a library is not proof of execution on GPU.
3. Historical [duplicate-source audit](https://github.com/rafaelmeloreisnovo/termux-app-rafacodephi/blob/d56d18e57dc2a358d8fb215a0e8687fce18b8ee0/reports/duplicate_source_audit.md) reports 31 duplicate groups/50 excess copies (as of that report). Canonical owner and hashes must be reconciled at chosen HEAD; copies do not imply newer variants.
4. [Prior GBC/LayersBit/BitOmega audit](https://github.com/rafaelmeloreisnovo/CONVERSATIONS_CHUNKS_PRIVATE/blob/main/auditoria/vector_manifold_novoexport_v1/findings/2026-09-25_code_verification.md) identified source roles, while the actual call edges `LayersBit → BitOmega → RMR → GBS3` still require symbol/call graph and invocation receipts. No private conversation payload is copied here.
5. `B14` also appears in a *different* ChipQuantum block-14 HMAC-SHA256 suite; same label does not imply continuation of `rafaelia_b8.S`. Promotable lineage requires commit ancestry + byte/blob hashes + behavior + tests.
6. Architecture registry is SOURCE_BOUND_SEED, not exhaustive. Next index pass should route unresolved terms through permitted Drive shards (metadata first), Git default and historical refs, producing typed `SOURCE_POINTER`, `ALIAS`, `VERSION_OF`, `DERIVED_FROM`, `COPY_OF`, `CONTRADICTS`, `UNRESOLVED`, with no guessed link, no private payload publication and no autopromotion.

Result for this PR9: `SOURCE_AUDIT_ONLY`, `INTEGRATION_NOT_RUN`, `NO_PROMOTION`. No L0 motor, Drive OAuth, Android runtime or remote GPU job is produced by this change.

## 1. Source-side components; do not duplicate

| Role | Existing producer and source | Verified scope / remaining gate |
| --- | --- | --- |
| Bounded deterministic NUMA adviser | [RafPolimata L0 NUMA](https://github.com/rafaelmeloreisnovo/RafPolimata/blob/8d0fb29904b0bbd990b8fec08c57e5ab05db42dd/freestanding/include/raf_fs_numa_plan.h) | Pure choice from caller-supplied snapshot; not actual NUMA placement |
| CRC-32 ZIP and STORE archive | [RafPolimata fmt_zip.h](https://github.com/rafaelmeloreisnovo/RafPolimata/blob/8d0fb29904b0bbd990b8fec08c57e5ab05db42dd/Apkc/fmt_zip.h) | Hardware CRC32 if available; software fallback; ZIP STORE, **not** Huffman |
| Java NIO / JNI buffer bridge | [termux-app RafaeliaCore.java](https://github.com/rafaelmeloreisnovo/termux-app-rafacodephi/blob/d56d18e57dc2a358d8fb215a0e8687fce18b8ee0/rafaelia/src/main/java/com/termux/rafaelia/RafaeliaCore.java) | 64 KiB direct buffers + JNI declarations; hosted ART; app/runtime evidence separate |
| BitWalk / BitGhost | [Vectras GPL-3.0-only header](https://github.com/rafaelmeloreisnovo/Vectras-VM-Android/blob/6e4d99c8d90a3a1ddcb68933c693815d7c6e847b/Rafaelia/rafaelia_bitwalk.h) | View-layer visibility; no deletion, decompression, data recovery, or permission bypass; legal/license integration gate |
| ZIPRAF prototype | [llamaRafaelia source](https://github.com/rafaelmeloreisnovo/llamaRafaelia/blob/b532b7678e4f86b1b5dccf5f54a6b310f9014b34/rafaelia-baremetal/zipraf/raf_zipraf.c) | libc/malloc/filesystem hosted dependencies; Huffman placeholder; do not claim L0 |
| Routes/indices | [Mapa repository](https://github.com/rafaelmeloreisnovo/Mapa) | Structural atlas, not measured Android hardware execution |

The platform adapter belongs to an authorized Android/Termux producer, L0 operations to a verified freestanding producer, shared semantic graph contracts to Mapa. `SETUP_RAFAELIA` only records control contracts, responsibility and gates. No producer is reassigned by this file.

## 2. Architecture boundary, not false zero-overhead claim

```text
Google Drive (authorized account + selected roots; files + revisions)
         | OAuth / HTTPS / TLS / Google Drive API (HOSTED L1)
         v
Android/ART broker (restricted access, paced I/O, lifecycle, retry)
         | caller-owned direct buffer, byte count, stable MIME/format
         v
C/ASM freestanding L0 (scalar required; SIMD/CRC feature-gated)
         | typed records, hashes, graph proposals, failure codes
         v
Android broker (validate -> stage -> provider write -> readback)
         |
Google Drive (immutable source + versioned index + μWRITE receipts)
```

`-ffreestanding` alone is not a runtime, linker, network stack, OAuth client, or GPU driver. Never claim zero overhead across JNI, ART, kernel syscalls, TLS or network. SDK/NDK/ART are hosted or build-time boundaries. Android OS schedules threads; the application still must decide work ordering, bounded in-flight memory, cancellation, backoff and transaction checkpoints. No app-specific scheduler, core pinning, or worker-thread explosion by default.

CPU L1/L2/L3 are **hardware-managed caches** (not ordinary application storage). Distinguish hardware caches from logical HOT/WARM/COLD tiers. NUMA must default `NOT_APPLICABLE_OR_UNPROVEN` on Android until physical topology is obtained; eight CPU cores are not eight NUMA nodes. SIMD NEON is an optional validated fast path; GPU requires platform/driver interface, kernel, and A/B device proof. CRC32 and CRC32C are different polynomials; ZIP requires CRC-32 IEEE, and CRC is not a cryptographic hash.

Two bytes encode sixteen **logical bits**. Packing into a 32-bit word leaves the other sixteen bits unless explicitly populated; it does not prove sixteen simultaneous physical ALUs. Validate scalar vs packed/SIMD equivalence with alignment/tail tests.

## 3. Drive librarian pipeline V1 (idempotent by identity, not name)

0. **Authorize**: account alias, exact allowed root IDs, allowed operations, privacy class, scope, human owner, rollback, quota and storage limit. Without verified `SOURCE|AUTHORITY|EXECUTION_TARGET|EVIDENCE_RULE`: `BLOCKED/TOKEN_VAZIO`.
1. **Bootstrap bounded**: enumerate only authorized roots, paginate, preserve native parent IDs, owners/access class, MIME, size, modifiedTime, revision/version, file ID and observed source locator. Recursion depth 1 initially; expansion by named reason only.
2. **Change feed**: after bounded initial inventory store Drive `changes.getStartPageToken` and consume `changes.list` per account/shared drive. Persist token only after complete page processing, readback and receipt; retry 429/5xx with bounded exponential backoff/jitter. A removal or 404 may be loss of permission, not deletion.
3. **Read minimal**: metadata first; when authorized and necessary fetch/export exact content according to MIME. For Google Docs, exported bytes are a representation of the native revision, not canonical file bytes. Capture `representation_type`. Skip oversized/unreadable/sensitive content with typed reason rather than silent drop. Do not train a model from this corpus.
4. **Classify deterministically**: extract title, path, allowed metadata and bounded text; graph proposal with `subject`, `source_locator`, `span/offset`, `extractor_version`, `confidence`, `visibility`, and relation type. Heuristic/semantic links are **proposals** until separately verified.
5. **Typed edges**: `REFERENCES`, `DERIVED_FROM`, `DUPLICATES_BYTES`, `SUPERSEDES`, `PARENT_OF`, `CONTRADICTS`, `TEMPORALLY_FOLLOWS`, `RELATES_TO`. Store claim status and supporting evidence for each edge. Unknown/non-verbal/inferred observations remain separately typed; no invented subject or author.
6. **Dedup without destruction**: first compare account+file_id+revision for same source version; `DUPLICATES_BYTES` only if actual byte representations and SHA-256 match. Same title, CRC, semantic similarity or filename is not sufficient. Keep duplicates as separate references unless separately authorized to reorganize.
7. **Stage then commit**: prepare new manifest + atlas/shards + receipt in private authorized Drive destination; validate schema, references, expected parents, SHA-256 when bytes observed; write append-only successor files; read back IDs/revisions/content. Never overwrite/delete originals or publicize private locators.
8. **Checkpoint**: receipt `source->representation->transform->artifact->readback` including `head/revision`, extractor version, counts, rejects, errors, next page token and rollback pointer. Advance `CURRENT_STATE` only after evidenced state transition. A failed stage leaves old index intact.

**Minimum record** (all fields typed):
```text
{account_alias, file_id, revision_or_version, parents[], mime_type,
 visibility, source_locator_private, representation_type,
 bytes_observed, sha256_or_TOKEN_VAZIO_HASH, extractor_version,
 extracted_nodes[], relation_proposals[], state, failure_reason,
 receipt_ref, supersedes_ref}
```
Metadata may live in Drive. Public GitHub stores only non-sensitive schemas/specs/tests and opaque authorized reference IDs.

## 4. Bit decision states and reconstructibility

A single bit has 0/1 only when observed. For uncertain bits, use separate `known_mask` and `value_bits`; a known 0 is `known_mask=1,value=0`, a known 1 is `known_mask=1,value=1`, unknown is `known_mask=0`. Track contradictory claims separately, not by selecting a convenient bit. ALLOW requires explicit eligible evidence; unknown/conflict must HOLD/DENY according to the policy (fail closed). Ghosted is a **view permission/visibility state**, not physical deletion or absence.

Huffman encoding is lossless when its codebook and encoded stream are kept; it does not permit arbitrary 25–45% bit deletion with guaranteed perfect recovery. Any 25–45% saving is a test hypothesis requiring original-bit equality on decompression, corpus, distribution, compression ratio (including dictionaries/metadata), missing-block injection, repair redundancy, and restoration receipt. CRC-32 detects many accidental modifications but neither repairs data nor authenticates it. Use SHA-256 and stronger attestation where required.

TTL may encode a logical expiry/counter. Actual elapsed time, wake-up scheduling and restarts require Android-monotonic/platform clock and lifecycle handling. Do not equate TTL tick with a guaranteed physical timer.

## 5. Acceptance falsifiers (all fail closed)

| Gate | Negative test | Required outcome |
| --- | --- | --- |
| G0 authority | wrong account/root, expired scope, unreadable ACL | no fetch/write; `BLOCKED` |
| G1 source | duplicate name but different IDs/revisions/bytes | no destructive dedup |
| G2 pagination | 429, partial page, failed checkpoint | no token advancement; safe retry |
| G3 format | wrong MIME, native-doc export mismatch, truncated chunk | typed error and no promotion |
| G4 bits | unknown=0 conflation, conflicting layer evidence | deny/HOLD; retain alternative propositions |
| G5 integrity | CRC32/CRC32C swapped, one-bit corruption, SHA mismatch | reject artifact, keep predecessor |
| G6 reconstruction | simulated 25%/45% loss without enough redundancy | FAIL, never reconstruct fabricated bytes |
| G7 device | ARMv7/ARM64 feature absent, misaligned tail, illegal instruction | scalar fallback, exact equality |
| G8 GPU/NUMA | inaccessible GPU / unknown NUMA topology | `NOT_RUN` / `TOKEN_VAZIO`, not PASS |
| G9 transaction | provider accepted write but readback differs, concurrent revision change | HOLD, receipt, successor only |
| G10 rights | upstream GPL source copied into incompatible destination | block licensing/promotion |
| G11 privacy | private path, credentials, snippets in public artifacts | reject/no provider write |
| G12 performance | CPU vs GPU/SIMD measured without common workload, thermal profile or exact binary | no speedup claim |

## 6. Exit requirements and next producer decision

1. Create a tiny *authorized* Drive folder fixture with synthetic, non-personal content, same-title/different-bytes, two revisions, unknown bit and 429/partial-page simulation.
2. Choose one Android/Termux producer and confirm source-owner approval; use existing JNI DirectByteBuffer or tested bounded buffer interface, not parallel duplicate stacks.
3. Implement scalar L0 with no libc/heap/syscalls then verify object undefined symbols and exact-byte reconstruction; add optional CRC32/NEON feature dispatch **after** scalar control.
4. Implement one provider-API-backed L1 connector under least-privilege OAuth; no credential storage in GitHub; checkpoint + retry + readback.
5. Same-HEAD CI and physical ARM32/ARM64 receipts. Measure memory, bytes read/written, thermal behavior, energy, failures, and data completeness before considering GPU/NUMA.
6. Only after source review, privacy gate, distinct approval, provider enforcement and test-negative readback can a promotion be considered.

Current state of this contract: `DOCUMENTED_PLAN_ONLY`, `ANDROID_PIPELINE_EXECUTION=NOT_RUN`, `DRIVE_CATALOG_COMPLETE=TOKEN_VAZIO`, `GPU_NUMA_PHYSICAL=TOKEN_VAZIO`, `claim_allowed=false`.
Rollback: close this draft PR or supersede this plan; existing Drive corpus, repository producers and provider settings remain untouched.
