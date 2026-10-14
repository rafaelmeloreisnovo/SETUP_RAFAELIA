/* RAFAELIA / SETUP — Drive librarian L0 incubator, ABI v1.
 * Pure freestanding C, caller-provided buffers, deterministic output.
 * Not a Google Drive client, Android app, secure signature or semantic judge.
 */
#ifndef RAF_LIBRARIAN_V1_H
#define RAF_LIBRARIAN_V1_H
enum {
 RAF_LBR_OK=0, RAF_LBR_BAD_ARG=1, RAF_LBR_BOUNDS=2, RAF_LBR_ORDER=3,
 RAF_LBR_EVIDENCE=4, RAF_LBR_RELATION=5, RAF_LBR_OVERFLOW=6
};
enum {
 RAF_LBR_DRIVE_ORIGIN=1, RAF_LBR_DRIVE_WITNESS=2, RAF_LBR_UNKNOWN_ORIGIN=3
};
enum {
 RAF_LBR_BYTES_VERIFIED=1, RAF_LBR_METADATA_ONLY=2, RAF_LBR_WITNESS_VERIFIED=3
};
enum {
 RAF_LBR_SAME_BYTES=1, RAF_LBR_PRECEDES=2,
 RAF_LBR_COOCCURS_CANDIDATE=3, RAF_LBR_DERIVED_FROM=4
};
enum {
 RAF_LBR_REL_OBSERVED=1, RAF_LBR_REL_DECLARED=2,
 RAF_LBR_REL_CANDIDATE=3, RAF_LBR_REL_TOKEN_VAZIO=4
};
typedef struct {
 unsigned int h[8];
 unsigned long long bytes;
 unsigned int used;
 unsigned char block[64];
} raf_lbr_sha256;
typedef struct {
 unsigned int ordinal,origin,evidence;
 unsigned long long size_bytes;
 unsigned int conversation_count,message_count;
 unsigned char locator_sha256[32],content_sha256[32];
} raf_lbr_item;
typedef struct {
 raf_lbr_sha256 stream;
 raf_lbr_item items[64];
 unsigned int count,links,held,unknown,conversations,messages,candidates;
} raf_lbr_batch;
typedef struct {
 unsigned char receipt_sha256[32];
 unsigned int count,links,held,unknown,conversations,messages,candidates;
 unsigned int promotion_allowed; /* Always zero: provider/readback review external. */
} raf_lbr_summary;
void raf_lbr_sha256_init(raf_lbr_sha256 *ctx);
int raf_lbr_sha256_update(raf_lbr_sha256 *ctx,
                           const unsigned char *data,unsigned int bytes);
int raf_lbr_sha256_final(const raf_lbr_sha256 *ctx,unsigned char digest[32]);
int raf_lbr_batch_init(raf_lbr_batch *batch);
int raf_lbr_batch_add(raf_lbr_batch *batch,const raf_lbr_item *item);
int raf_lbr_batch_link(raf_lbr_batch *batch,unsigned int a,unsigned int b,
                       unsigned int relation,unsigned int relation_state);
int raf_lbr_batch_finish(const raf_lbr_batch *batch,raf_lbr_summary *out);
#endif
