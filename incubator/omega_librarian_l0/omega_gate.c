/* SPDX-License-Identifier: LicenseRef-RAFCODE-Research-Commercial-0.1
 * Evidence and route decision only. No symbols outside this compilation unit.
 */
#include "omega_gate.h"
static unsigned int tri(unsigned int v) {
 return v==OMEGA_UNKNOWN || v==OMEGA_YES || v==OMEGA_NO;
}
static unsigned int decision(unsigned int e) {
 return e==OMEGA_NO ? OMEGA_DENY : OMEGA_HOLD;
}
unsigned int omega_gate_decide(const omega_gate_input *in,
                                omega_gate_output *out) {
 omega_gate_output o;
 unsigned int i;
 const unsigned int *p;
 if (!in || !out) return OMEGA_BAD_INPUT;
 if (in->version!=1u || in->action<OMEGA_READ ||
     in->action>OMEGA_PROMOTE || in->workload_bytes>0x7fffffffu)
  return OMEGA_BAD_INPUT;
 /* All tri-state evidence fields must be valid before a decision. */
 p=&in->source_identified;
 for (i=0u;i<12u;++i) {
  /* Fixed explicitly laid out adjacent unsigned fields only.
   * Avoid pointer arithmetic over structs: inspect by switch below instead. */
  unsigned int v;
  switch(i) {
   case 0u: v=in->source_identified; break;
   case 1u: v=in->read_authority; break;
   case 2u: v=in->source_sha256_verified; break;
   case 3u: v=in->crc32_verified; break;
   case 4u: v=in->destination_authorized; break;
   case 5u: v=in->redaction_verified; break;
   case 6u: v=in->write_receipt_verified; break;
   case 7u: v=in->readback_verified; break;
   case 8u: v=in->provider_gate_verified; break;
   case 9u: v=in->erasure_present; break;
   case 10u: v=in->recovery_verified; break;
   default: v=OMEGA_UNKNOWN; break;
  }
  if (!tri(v)) return OMEGA_BAD_INPUT;
 }
 (void)p;
 if ((in->attested_features & ~in->advertised_features)!=0u)
  return OMEGA_BAD_INPUT;
 o.decision=OMEGA_ALLOW; o.reason=0u; o.route=OMEGA_SCALAR;
 o.crc_route=OMEGA_SCALAR;
 if (in->source_identified!=OMEGA_YES) {
  o.decision=decision(in->source_identified); o.reason=OMEGA_R_SOURCE;
 } else if (in->read_authority!=OMEGA_YES) {
  o.decision=decision(in->read_authority); o.reason=OMEGA_R_ACL;
 } else if (!in->epoch || in->epoch!=in->expected_epoch) {
  o.decision=OMEGA_HOLD; o.reason=OMEGA_R_EPOCH;
 } else if (in->expires_tick<=in->now_tick) {
  o.decision=OMEGA_HOLD; o.reason=OMEGA_R_TTL;
 } else if (in->action>=OMEGA_INDEX &&
           in->source_sha256_verified!=OMEGA_YES) {
  o.decision=decision(in->source_sha256_verified); o.reason=OMEGA_R_DIGEST;
 } else if (in->crc32_verified==OMEGA_NO) {
  o.decision=OMEGA_DENY; o.reason=OMEGA_R_DIGEST;
 } else if (in->erasure_present!=OMEGA_NO &&
           (in->erasure_present==OMEGA_UNKNOWN ||
            in->recovery_verified!=OMEGA_YES)) {
  o.decision=OMEGA_HOLD; o.reason=OMEGA_R_ERASURE;
 } else if (in->action>=OMEGA_WRITE &&
            (in->destination_authorized!=OMEGA_YES ||
             in->redaction_verified!=OMEGA_YES)) {
  o.decision=decision(in->destination_authorized);
  if (in->destination_authorized==OMEGA_YES)
   o.decision=decision(in->redaction_verified);
  o.reason=OMEGA_R_PRIVATE;
 } else if (in->action==OMEGA_PROMOTE &&
            (in->write_receipt_verified!=OMEGA_YES ||
             in->readback_verified!=OMEGA_YES)) {
  o.decision=OMEGA_HOLD; o.reason=OMEGA_R_RECEIPT;
 } else if (in->action==OMEGA_PROMOTE &&
            in->provider_gate_verified!=OMEGA_YES) {
  o.decision=OMEGA_HOLD; o.reason=OMEGA_R_PROVIDER;
 }
 if (o.decision==OMEGA_ALLOW) {
  if ((in->attested_features & OMEGA_NEON) && in->workload_bytes>=4096u)
   o.route=OMEGA_NEON;
  if (in->attested_features & OMEGA_ARM_CRC32)
   o.crc_route=OMEGA_ARM_CRC32;
 }
 *out=o;
 return o.decision;
}
