/* RAFAELIA Drive Librarian L0: pure state + streaming integrity, V1.
 * SPDX-License-Identifier: MIT
 * This file does not implement network I/O, authentication or SHA-256.
 * The Drive/Android adapter owns those capabilities.
 */
#ifndef RAF_DRIVE_LIBRARIAN_L0_H
#define RAF_DRIVE_LIBRARIAN_L0_H
#if !defined(__CHAR_BIT__) || __CHAR_BIT__ != 8
#error "Requires 8-bit bytes"
#endif
typedef unsigned char raf_bib_u8;
typedef unsigned int raf_bib_u32;
typedef unsigned long long raf_bib_u64;
typedef char raf_bib_check32[(sizeof(raf_bib_u32)==4)?1:-1];
typedef char raf_bib_check64[(sizeof(raf_bib_u64)==8)?1:-1];

#define RAF_BIB_MAX_BLOCK 4096U
#define RAF_BIB_EMPTY 0U
#define RAF_BIB_READING 1U
#define RAF_BIB_STAGED 2U
#define RAF_BIB_ACKED 3U
#define RAF_BIB_OK 0U
#define RAF_BIB_BAD_ARGUMENT 1U
#define RAF_BIB_BAD_PHASE 2U
#define RAF_BIB_BOUNDS 3U
#define RAF_BIB_OUT_OF_ORDER 4U
#define RAF_BIB_SHA_MISMATCH 5U
#define RAF_BIB_MISSING_ATTESTATION 6U
#define RAF_BIB_ALLOW 1U
#define RAF_BIB_DENY 0U
#define RAF_BIB_UNKNOWN 2U

/* A reference is merely a stable, caller-supplied locator fingerprint.
 * present=1 is separate from value=0; no confusion of UNKNOWN and zero. */
typedef struct {
 raf_bib_u64 locator_fp;
 raf_bib_u64 revision_fp;
 raf_bib_u32 present;
} raf_bib_ref;

typedef struct {
 raf_bib_u64 total_bytes;
 raf_bib_u64 consumed_bytes;
 raf_bib_u32 crc_internal;
 raf_bib_u32 block_count;
 raf_bib_u32 phase;
 raf_bib_ref source;
 raf_bib_ref receipt_target;
 raf_bib_u8 expected_source_sha256[32];
 raf_bib_u8 staged_receipt_sha256[32];
} raf_bib_state;

/* Every error is fail-closed and leaves *state unchanged. No global state,
 * heap, libc, syscalls, OpenSSL or Android SDK in this module. */
raf_bib_u32 raf_bib_begin(raf_bib_state *state, const raf_bib_ref *source,
                          raf_bib_u64 expected_bytes,
                          const raf_bib_u8 expected_sha256[32]);
raf_bib_u32 raf_bib_feed(raf_bib_state *state, const raf_bib_u8 *bytes,
                         raf_bib_u32 count, raf_bib_u64 expected_offset);
raf_bib_u32 raf_bib_stage(raf_bib_state *state,
                          const raf_bib_u8 source_sha256_from_adapter[32],
                          const raf_bib_u8 staged_receipt_sha256[32],
                          raf_bib_u32 source_revision_readback_ok);
raf_bib_u32 raf_bib_ack(raf_bib_state *state, const raf_bib_ref *target,
                        const raf_bib_u8 receipt_sha256_from_drive_readback[32],
                        raf_bib_u32 target_revision_readback_ok);
raf_bib_u32 raf_bib_crc32(const raf_bib_state *state);
raf_bib_u32 raf_bib_decision(const raf_bib_state *state);
#endif
