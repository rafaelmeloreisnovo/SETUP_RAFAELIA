/* SPDX-License-Identifier: LicenseRef-RAFCODE-Research-Commercial-0.1
 * RAFAELIA Ω librarian L0: bounded evidence gate, freestanding C, NO I/O.
 * Does not perform OAuth, Drive read/write, scheduling, CRC, ECC or GPU calls.
 */
#ifndef RAFAELIA_OMEGA_GATE_H
#define RAFAELIA_OMEGA_GATE_H
#if !defined(__CHAR_BIT__) || __CHAR_BIT__ != 8
#error "Requires 8-bit bytes"
#endif
typedef char omega_u32_is_4_bytes[(sizeof(unsigned int)==4) ? 1 : -1];
#define OMEGA_UNKNOWN 0u
#define OMEGA_YES 1u
#define OMEGA_NO 2u
#define OMEGA_READ 1u
#define OMEGA_INDEX 2u
#define OMEGA_WRITE 3u
#define OMEGA_PROMOTE 4u
#define OMEGA_ALLOW 1u
#define OMEGA_DENY 2u
#define OMEGA_HOLD 3u
#define OMEGA_BAD_INPUT 4u
#define OMEGA_SCALAR 1u
#define OMEGA_NEON 2u
#define OMEGA_ARM_CRC32 4u
#define OMEGA_GPU 8u
#define OMEGA_NUMA 16u
#define OMEGA_HOST_NIO 32u
#define OMEGA_R_INPUT 1u
#define OMEGA_R_SOURCE 2u
#define OMEGA_R_ACL 3u
#define OMEGA_R_EPOCH 4u
#define OMEGA_R_TTL 5u
#define OMEGA_R_DIGEST 6u
#define OMEGA_R_ERASURE 7u
#define OMEGA_R_PRIVATE 8u
#define OMEGA_R_RECEIPT 9u
#define OMEGA_R_PROVIDER 10u
typedef struct omega_gate_input {
 unsigned int version; /* 1 */
 unsigned int action; /* READ, INDEX, WRITE, PROMOTE */
 unsigned int source_identified; /* 0 unknown, 1 yes, 2 no */
 unsigned int read_authority;
 unsigned int source_sha256_verified; /* scope exact bytes by host */
 unsigned int crc32_verified; /* optional, never authentication */
 unsigned int epoch; /* supplied by the host; nonzero if known */
 unsigned int expected_epoch;
 unsigned int now_tick; /* same monotonic timescale as expires_tick */
 unsigned int expires_tick; /* TTL is lease deadline, not hardware timer */
 unsigned int destination_authorized;
 unsigned int redaction_verified; /* false if user texts may be exported */
 unsigned int write_receipt_verified;
 unsigned int readback_verified;
 unsigned int provider_gate_verified;
 unsigned int erasure_present;
 unsigned int recovery_verified; /* verified FEC+exact digest, not assumption */
 unsigned int advertised_features; /* host feature detection only */
 unsigned int attested_features; /* tested route subset */
 unsigned int workload_bytes; /* bounded, host supplies size */
} omega_gate_input;
typedef struct omega_gate_output {
 unsigned int decision;
 unsigned int reason;
 unsigned int route; /* SCALAR default; NEON only after attestation */
 unsigned int crc_route; /* ARM_CRC32 if attested; else SCALAR */
} omega_gate_output;
/* On BAD_INPUT output stays untouched. HOLD/ DENY are distinct states.
 * Callers must never map HOLD=0 or interpret ALLOW as a proof of Drive write.
 */
unsigned int omega_gate_decide(const omega_gate_input *in,
                                omega_gate_output *out);
#endif
