/* Hosted falsifier only. Freestanding L0 never imports stdio or libc.
 * SPDX-License-Identifier: LicenseRef-RAFCODE-Research-Commercial-0.1
 */
#include <stdio.h>
#include "omega_gate.h"
#define EQ(name, actual, expected) do { if ((actual)!=(expected)) {  printf("FAIL %s: got=%u expected=%u\n",name,(unsigned)(actual),(unsigned)(expected));  return 1; } } while(0)
static omega_gate_input valid(void) {
 omega_gate_input x={0};
 x.version=1u;x.action=OMEGA_INDEX;
 x.source_identified=OMEGA_YES;x.read_authority=OMEGA_YES;
 x.source_sha256_verified=OMEGA_YES;x.crc32_verified=OMEGA_UNKNOWN;
 x.epoch=9u;x.expected_epoch=9u;x.now_tick=12u;x.expires_tick=15u;
 x.destination_authorized=OMEGA_UNKNOWN;x.redaction_verified=OMEGA_UNKNOWN;
 x.write_receipt_verified=OMEGA_UNKNOWN;x.readback_verified=OMEGA_UNKNOWN;
 x.provider_gate_verified=OMEGA_UNKNOWN;x.erasure_present=OMEGA_NO;
 x.recovery_verified=OMEGA_UNKNOWN;x.workload_bytes=4096u;
 x.advertised_features=OMEGA_NEON|OMEGA_ARM_CRC32|OMEGA_GPU|OMEGA_NUMA|OMEGA_HOST_NIO;
 x.attested_features=0u;
 return x;
}
int main(void) {
 omega_gate_input x=valid();
 omega_gate_output o={99u,88u,77u,66u};
 unsigned int r;
 r=omega_gate_decide(&x,&o);EQ("base",r,OMEGA_ALLOW);
 EQ("base_scalar",o.route,OMEGA_SCALAR);
 x.source_identified=OMEGA_UNKNOWN;
 EQ("unknown_source",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.source_identified=OMEGA_NO;
 EQ("explicit_denial",omega_gate_decide(&x,&o),OMEGA_DENY);
 x.source_identified=OMEGA_YES;x.read_authority=OMEGA_NO;
 EQ("acl_deny",omega_gate_decide(&x,&o),OMEGA_DENY);
 x.read_authority=OMEGA_YES;x.source_sha256_verified=OMEGA_UNKNOWN;
 EQ("unknown_sha",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.source_sha256_verified=OMEGA_NO;
 EQ("bad_sha",omega_gate_decide(&x,&o),OMEGA_DENY);
 x.source_sha256_verified=OMEGA_YES;x.crc32_verified=OMEGA_NO;
 EQ("crc32_mismatch",omega_gate_decide(&x,&o),OMEGA_DENY);
 x.crc32_verified=OMEGA_UNKNOWN;x.expected_epoch=8u;
 EQ("stale_epoch",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.expected_epoch=9u;x.now_tick=15u;
 EQ("ttl_expired",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.now_tick=12u;x.erasure_present=OMEGA_UNKNOWN;
 EQ("unknown_erasures",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.erasure_present=OMEGA_YES;
 EQ("unproven_recovery",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.recovery_verified=OMEGA_YES;
 EQ("only_if_verified_recovery",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 x.erasure_present=OMEGA_NO;x.action=OMEGA_READ;
 x.source_sha256_verified=OMEGA_UNKNOWN;
 EQ("read_before_sha",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 x.source_sha256_verified=OMEGA_YES;x.action=OMEGA_WRITE;
 EQ("write_unknown_authority",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.destination_authorized=OMEGA_YES;x.redaction_verified=OMEGA_NO;
 EQ("leakage_denied",omega_gate_decide(&x,&o),OMEGA_DENY);
 x.redaction_verified=OMEGA_YES;
 EQ("write_preparation",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 x.action=OMEGA_PROMOTE;
 EQ("no_receipt_no_promote",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.write_receipt_verified=OMEGA_YES;x.readback_verified=OMEGA_YES;
 EQ("provider_unverified",omega_gate_decide(&x,&o),OMEGA_HOLD);
 x.provider_gate_verified=OMEGA_YES;
 EQ("all_promote_gates",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 x.action=OMEGA_INDEX;x.attested_features=OMEGA_NEON;
 EQ("attested_neon",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 EQ("neon_route",o.route,OMEGA_NEON);
 x.workload_bytes=4095u;
 EQ("small_scalar",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 EQ("small_route",o.route,OMEGA_SCALAR);
 x.workload_bytes=4096u;x.attested_features=OMEGA_ARM_CRC32;
 EQ("crc_route_decision",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 EQ("crc_hardware",o.crc_route,OMEGA_ARM_CRC32);
 EQ("crc_not_neon",o.route,OMEGA_SCALAR);
 x.attested_features=OMEGA_GPU|OMEGA_NUMA;
 EQ("no_gpu_or_numa_auto",omega_gate_decide(&x,&o),OMEGA_ALLOW);
 EQ("gpu_cpu_fallback",o.route,OMEGA_SCALAR);
 x.attested_features=0x80000000u;
 o.decision=99u;o.reason=88u;o.route=77u;o.crc_route=66u;
 EQ("capability_spoof",omega_gate_decide(&x,&o),OMEGA_BAD_INPUT);
 EQ("no_output_clobber_on_bad_input",o.decision,99u);
 /* Previous V1 passed a forged future bit when BOTH masks contained it.
  * Unknown architecture capability != proven execution or authorization. */
 x.attested_features=0x40000000u;
 x.advertised_features=0x40000000u;
 o.decision=99u;o.reason=88u;o.route=77u;o.crc_route=66u;
 EQ("unrecognized_but_attested_feature",omega_gate_decide(&x,&o),OMEGA_BAD_INPUT);
 EQ("unrecognized_feature_no_output_write",o.decision,99u);
 x.attested_features=0u;
 o.decision=99u;
 EQ("unrecognized_advertised_feature",omega_gate_decide(&x,&o),OMEGA_BAD_INPUT);
 EQ("unrecognized_advertised_no_output_write",o.decision,99u);
 x.advertised_features=OMEGA_NEON|OMEGA_ARM_CRC32|OMEGA_GPU|OMEGA_NUMA|OMEGA_HOST_NIO;
 x.attested_features=0u;x.source_identified=3u;
 EQ("tri_state_invalid",omega_gate_decide(&x,&o),OMEGA_BAD_INPUT);
 x.source_identified=OMEGA_YES;x.version=3u;
 EQ("schema_mismatch",omega_gate_decide(&x,&o),OMEGA_BAD_INPUT);
 EQ("null_input",omega_gate_decide(0,&o),OMEGA_BAD_INPUT);
 puts("OMEGA_L0_POLICY_HOST_SYNTHETIC_NEGATIVES=PASS_SCOPED");
 puts("DRIVE_OAUTH_GPU_NUMA_HARDWARE_ECC_PHYSICAL=NOT_RUN");
 return 0;
}
