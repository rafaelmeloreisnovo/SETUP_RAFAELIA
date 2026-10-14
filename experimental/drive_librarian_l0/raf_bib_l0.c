/* RAFAELIA Drive Librarian L0 V1.
 * SPDX-License-Identifier: MIT
 * Portable IEEE CRC32 (ZIP polynomial), reference contract aligned to
 * RafPolimata/tools/rafbbs/rafbbs_crc32_core.h; independently written.
 * CRC is error detection, NOT SHA-256, ECC or encryption.
 */
#include "raf_bib_l0.h"
static void raf_copy32(raf_bib_u8 *dst, const raf_bib_u8 *src) {
 raf_bib_u32 i;
 for (i=0U;i<32U;++i) dst[i]=src[i];
}
static raf_bib_u32 raf_equal32(const raf_bib_u8 *a,const raf_bib_u8 *b) {
 raf_bib_u32 i,diff=0U;
 for (i=0U;i<32U;++i) diff|=(raf_bib_u32)(a[i]^b[i]);
 return (raf_bib_u32)(diff==0U);
}
raf_bib_u32 raf_bib_begin(raf_bib_state *state,const raf_bib_ref *source,
                          raf_bib_u64 expected_bytes,
                          const raf_bib_u8 expected_sha256[32]) {
 raf_bib_state next;
 if (!state || !source || !expected_sha256 || source->present!=1U)
  return RAF_BIB_BAD_ARGUMENT;
 next.total_bytes=expected_bytes; next.consumed_bytes=0ULL;
 next.crc_internal=0xFFFFFFFFU;next.block_count=0U;
 next.phase=RAF_BIB_READING;next.source=*source;
 next.receipt_target.locator_fp=0ULL;
 next.receipt_target.revision_fp=0ULL;
 next.receipt_target.present=0U;
 raf_copy32(next.expected_source_sha256,expected_sha256);
 { raf_bib_u32 i; for(i=0U;i<32U;++i)next.staged_receipt_sha256[i]=0U; }
 *state=next;
 return RAF_BIB_OK;
}
raf_bib_u32 raf_bib_feed(raf_bib_state *state,const raf_bib_u8 *bytes,
                         raf_bib_u32 count,raf_bib_u64 expected_offset) {
 raf_bib_u32 i,bit,c;
 if (!state || !bytes || !count) return RAF_BIB_BAD_ARGUMENT;
 if (state->phase!=RAF_BIB_READING) return RAF_BIB_BAD_PHASE;
 if (count>RAF_BIB_MAX_BLOCK || state->consumed_bytes>state->total_bytes)
  return RAF_BIB_BOUNDS;
 if (expected_offset!=state->consumed_bytes) return RAF_BIB_OUT_OF_ORDER;
 if ((raf_bib_u64)count>state->total_bytes-state->consumed_bytes ||
     state->block_count==0xFFFFFFFFU) return RAF_BIB_BOUNDS;
 c=state->crc_internal;
 for(i=0U;i<count;++i) {
  c^=(raf_bib_u32)bytes[i];
  for(bit=0U;bit<8U;++bit) {
   raf_bib_u32 mask=(raf_bib_u32)(0U-(c&1U));
   c=(c>>1)^(0xEDB88320U&mask);
  }
 }
 state->crc_internal=c;
 state->consumed_bytes+=(raf_bib_u64)count;
 ++state->block_count;
 return RAF_BIB_OK;
}
raf_bib_u32 raf_bib_stage(raf_bib_state *state,
                          const raf_bib_u8 source_sha256_from_adapter[32],
                          const raf_bib_u8 staged_receipt_sha256[32],
                          raf_bib_u32 source_revision_readback_ok) {
 if (!state || !source_sha256_from_adapter || !staged_receipt_sha256)
  return RAF_BIB_BAD_ARGUMENT;
 if(state->phase!=RAF_BIB_READING) return RAF_BIB_BAD_PHASE;
 if(state->consumed_bytes!=state->total_bytes) return RAF_BIB_BOUNDS;
 if(source_revision_readback_ok!=1U) return RAF_BIB_MISSING_ATTESTATION;
 if(!raf_equal32(state->expected_source_sha256,source_sha256_from_adapter))
  return RAF_BIB_SHA_MISMATCH;
 raf_copy32(state->staged_receipt_sha256,staged_receipt_sha256);
 state->phase=RAF_BIB_STAGED;
 return RAF_BIB_OK;
}
raf_bib_u32 raf_bib_ack(raf_bib_state *state,const raf_bib_ref *target,
                        const raf_bib_u8 receipt_sha256_from_drive_readback[32],
                        raf_bib_u32 target_revision_readback_ok) {
 if(!state || !target || !receipt_sha256_from_drive_readback ||
    target->present!=1U) return RAF_BIB_BAD_ARGUMENT;
 if(state->phase!=RAF_BIB_STAGED) return RAF_BIB_BAD_PHASE;
 if(target_revision_readback_ok!=1U) return RAF_BIB_MISSING_ATTESTATION;
 if(!raf_equal32(state->staged_receipt_sha256,
                 receipt_sha256_from_drive_readback))
  return RAF_BIB_SHA_MISMATCH;
 state->receipt_target=*target;
 state->phase=RAF_BIB_ACKED;
 return RAF_BIB_OK;
}
raf_bib_u32 raf_bib_crc32(const raf_bib_state *state) {
 if(!state || state->phase==RAF_BIB_EMPTY ||
    state->phase>RAF_BIB_ACKED) return 0U;
 return state->crc_internal^0xFFFFFFFFU;
}
raf_bib_u32 raf_bib_decision(const raf_bib_state *state) {
 if(!state || state->phase>RAF_BIB_ACKED)
  return RAF_BIB_DENY;
 return state->phase==RAF_BIB_ACKED?RAF_BIB_ALLOW:RAF_BIB_UNKNOWN;
}
