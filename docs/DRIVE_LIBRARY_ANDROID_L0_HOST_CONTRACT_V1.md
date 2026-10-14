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
