/* Hosted falsifier for the freestanding Drive Librarian L0 state core.
 * SPDX-License-Identifier: MIT
 * This harness may use libc; raf_bib_l0.c may not.
 */
#include <stdio.h>
#include <string.h>
#include "raf_bib_l0.h"
#define REQUIRE(c) do { if(!(c)) { printf("FAIL_LINE:%u\n",(unsigned)__LINE__);return 1;} } while(0)
static void fill(unsigned char *b,unsigned char seed) {
 unsigned int i;for(i=0U;i<32U;++i)b[i]=(unsigned char)(seed+(unsigned char)i);
}
int main(void) {
 const unsigned char known[]="123456789";
 unsigned char sha[32],bad[32],receipt[32],buf[4097U];
 raf_bib_ref source={0ULL,0ULL,1U},target={5ULL,6ULL,1U};
 raf_bib_state ctx,before,empty;
 unsigned int i;
 fill(sha,0x31U); fill(receipt,0x61U);fill(bad,0x11U);
 memset(&ctx,0,sizeof(ctx));
 REQUIRE(raf_bib_begin(&ctx,&source,9ULL,sha)==RAF_BIB_OK);
 REQUIRE(raf_bib_decision(&ctx)==RAF_BIB_UNKNOWN);
 REQUIRE(raf_bib_feed(&ctx,known,4U,0ULL)==RAF_BIB_OK);
 REQUIRE(raf_bib_feed(&ctx,known+4,5U,4ULL)==RAF_BIB_OK);
 REQUIRE(ctx.consumed_bytes==9ULL && ctx.block_count==2U);
 REQUIRE(raf_bib_crc32(&ctx)==0xCBF43926U); /* ZIP/IEEE CRC32 */
 before=ctx;
 REQUIRE(raf_bib_feed(&ctx,known,1U,2ULL)==RAF_BIB_OUT_OF_ORDER);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 REQUIRE(raf_bib_feed(&ctx,buf,4097U,9ULL)==RAF_BIB_BOUNDS);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 REQUIRE(raf_bib_stage(&ctx,bad,receipt,1U)==RAF_BIB_SHA_MISMATCH);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 REQUIRE(raf_bib_stage(&ctx,sha,receipt,0U)==RAF_BIB_MISSING_ATTESTATION);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 REQUIRE(raf_bib_stage(&ctx,sha,receipt,1U)==RAF_BIB_OK);
 REQUIRE(raf_bib_decision(&ctx)==RAF_BIB_UNKNOWN);
 before=ctx;
 REQUIRE(raf_bib_ack(&ctx,&target,bad,1U)==RAF_BIB_SHA_MISMATCH);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 REQUIRE(raf_bib_ack(&ctx,&target,receipt,0U)==RAF_BIB_MISSING_ATTESTATION);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 REQUIRE(raf_bib_ack(&ctx,&target,receipt,1U)==RAF_BIB_OK);
 REQUIRE(ctx.phase==RAF_BIB_ACKED && raf_bib_decision(&ctx)==RAF_BIB_ALLOW);
 before=ctx;
 REQUIRE(raf_bib_ack(&ctx,&target,receipt,1U)==RAF_BIB_BAD_PHASE);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 REQUIRE(raf_bib_feed(&ctx,known,1U,9ULL)==RAF_BIB_BAD_PHASE);
 REQUIRE(memcmp(&ctx,&before,sizeof(ctx))==0);
 /* Truncated raw source must never generate a receipt. */
 memset(&empty,0,sizeof(empty));
 REQUIRE(raf_bib_begin(&empty,&source,9ULL,sha)==RAF_BIB_OK);
 REQUIRE(raf_bib_feed(&empty,known,8U,0ULL)==RAF_BIB_OK);
 before=empty;
 REQUIRE(raf_bib_stage(&empty,sha,receipt,1U)==RAF_BIB_BOUNDS);
 REQUIRE(memcmp(&empty,&before,sizeof(empty))==0);
 REQUIRE(raf_bib_decision(&empty)==RAF_BIB_UNKNOWN);
 /* Unknown identity and the numeric value zero are distinguishable. */
 source.present=0U;memset(&empty,0,sizeof(empty));before=empty;
 REQUIRE(raf_bib_begin(&empty,&source,0ULL,sha)==RAF_BIB_BAD_ARGUMENT);
 REQUIRE(memcmp(&empty,&before,sizeof(empty))==0);
 source.present=1U; /* zero fingerprint is valid once expressly present */
 REQUIRE(raf_bib_begin(&empty,&source,0ULL,sha)==RAF_BIB_OK);
 REQUIRE(raf_bib_crc32(&empty)==0U);
 REQUIRE(raf_bib_stage(&empty,sha,receipt,1U)==RAF_BIB_OK);
 REQUIRE(raf_bib_ack(&empty,&target,receipt,1U)==RAF_BIB_OK);
 REQUIRE(raf_bib_decision(&empty)==RAF_BIB_ALLOW);
 /* Incremental buffer composition agrees with scalar checksum. */
 memset(&ctx,0,sizeof(ctx));
 REQUIRE(raf_bib_begin(&ctx,&source,4096ULL,sha)==RAF_BIB_OK);
 for(i=0U;i<4096U;++i)buf[i]=(unsigned char)(i&255U);
 REQUIRE(raf_bib_feed(&ctx,buf,4096U,0ULL)==RAF_BIB_OK);
 memset(&empty,0,sizeof(empty));
 REQUIRE(raf_bib_begin(&empty,&source,4096ULL,sha)==RAF_BIB_OK);
 for(i=0U;i<16U;++i)
  REQUIRE(raf_bib_feed(&empty,buf+(i*256U),256U,(raf_bib_u64)i*256ULL)==RAF_BIB_OK);
 REQUIRE(raf_bib_crc32(&ctx)==raf_bib_crc32(&empty));
 REQUIRE(raf_bib_decision((const raf_bib_state *)0)==RAF_BIB_DENY);
 puts("RAF_DRIVE_LIBRARIAN_L0_CRC32_IEEE=PASS_SCOPED");
 puts("RAF_DRIVE_LIBRARIAN_L0_CHUNK_16X256_EQ_4096=PASS_SCOPED");
 puts("RAF_DRIVE_LIBRARIAN_L0_GATES_REPLAY_TRUNCATION_SHA_REJECT=PASS_SCOPED");
 puts("RAF_DRIVE_LIBRARIAN_L0_REAL_DRIVE_AUTH_OR_ANDROID=NOT_RUN");
 return 0;
}
