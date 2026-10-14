/* RAFAELIA — SHA-256 and compact custody event ledger (freestanding C).
 * This is a local integrity primitive, not user identity authentication,
 * cloud authorization, semantic proof, or an assertion of zero overhead.
 */
#include "raf_librarian.h"
typedef unsigned int u32;
typedef unsigned long long u64;
_Static_assert(sizeof(u32)==4, "requires 32-bit unsigned int");
_Static_assert(sizeof(u64)==8, "requires 64-bit unsigned long long");
static const u32 k256[64]={
 0x428a2f98U,0x71374491U,0xb5c0fbcfU,0xe9b5dba5U,0x3956c25bU,0x59f111f1U,0x923f82a4U,0xab1c5ed5U,
 0xd807aa98U,0x12835b01U,0x243185beU,0x550c7dc3U,0x72be5d74U,0x80deb1feU,0x9bdc06a7U,0xc19bf174U,
 0xe49b69c1U,0xefbe4786U,0x0fc19dc6U,0x240ca1ccU,0x2de92c6fU,0x4a7484aaU,0x5cb0a9dcU,0x76f988daU,
 0x983e5152U,0xa831c66dU,0xb00327c8U,0xbf597fc7U,0xc6e00bf3U,0xd5a79147U,0x06ca6351U,0x14292967U,
 0x27b70a85U,0x2e1b2138U,0x4d2c6dfcU,0x53380d13U,0x650a7354U,0x766a0abbU,0x81c2c92eU,0x92722c85U,
 0xa2bfe8a1U,0xa81a664bU,0xc24b8b70U,0xc76c51a3U,0xd192e819U,0xd6990624U,0xf40e3585U,0x106aa070U,
 0x19a4c116U,0x1e376c08U,0x2748774cU,0x34b0bcb5U,0x391c0cb3U,0x4ed8aa4aU,0x5b9cca4fU,0x682e6ff3U,
 0x748f82eeU,0x78a5636fU,0x84c87814U,0x8cc70208U,0x90befffaU,0xa4506cebU,0xbef9a3f7U,0xc67178f2U
};
static u32 rr(u32 n,u32 bits){return (n>>bits)|(n<<(32U-bits));}
static void sha_block(raf_lbr_sha256 *c) {
 u32 w[64],a,b,d,e,f,g,h,cc,i,p,q,x,y;
 for(i=0;i<16U;++i) {
  w[i]=((u32)c->block[i*4U]<<24)|((u32)c->block[i*4U+1U]<<16)|
       ((u32)c->block[i*4U+2U]<<8)|((u32)c->block[i*4U+3U]);
 }
 for(i=16U;i<64U;++i){
  x=rr(w[i-15U],7U)^rr(w[i-15U],18U)^(w[i-15U]>>3);
  y=rr(w[i-2U],17U)^rr(w[i-2U],19U)^(w[i-2U]>>10);
  w[i]=w[i-16U]+x+w[i-7U]+y;
 }
 a=c->h[0];b=c->h[1];cc=c->h[2];d=c->h[3];
 e=c->h[4];f=c->h[5];g=c->h[6];h=c->h[7];
 for(i=0;i<64U;++i){
  p=h+(rr(e,6U)^rr(e,11U)^rr(e,25U))+
      ((e&f)^(~e&g))+k256[i]+w[i];
  q=(rr(a,2U)^rr(a,13U)^rr(a,22U))+((a&b)^(a&cc)^(b&cc));
  h=g;g=f;f=e;e=d+p;d=cc;cc=b;b=a;a=p+q;
 }
 c->h[0]+=a;c->h[1]+=b;c->h[2]+=cc;c->h[3]+=d;
 c->h[4]+=e;c->h[5]+=f;c->h[6]+=g;c->h[7]+=h;
}
void raf_lbr_sha256_init(raf_lbr_sha256 *c){
 static const u32 initial[8]={0x6a09e667U,0xbb67ae85U,0x3c6ef372U,0xa54ff53aU,
                               0x510e527fU,0x9b05688cU,0x1f83d9abU,0x5be0cd19U};
 u32 i;
 if(!c)return;
 for(i=0;i<8U;++i)c->h[i]=initial[i];
 c->bytes=0ULL;c->used=0U;
 for(i=0;i<64U;++i)c->block[i]=0;
}
int raf_lbr_sha256_update(raf_lbr_sha256 *c,const unsigned char *p,u32 n){
 u32 i;
 if(!c||(!p&&n))return RAF_LBR_BAD_ARG;
 if(c->used>=64U||c->bytes>0x1fffffffffffffffULL||
    (u64)n>0x1fffffffffffffffULL-c->bytes)return RAF_LBR_BOUNDS;
 for(i=0;i<n;++i) {
  c->block[c->used++]=p[i];
  if(c->used==64U){sha_block(c);c->used=0U;}
 }
 c->bytes+=(u64)n;
 return RAF_LBR_OK;
}
int raf_lbr_sha256_final(const raf_lbr_sha256 *ctx,unsigned char digest[32]){
 raf_lbr_sha256 c;u64 bitlen;u32 i;
 if(!ctx||!digest)return RAF_LBR_BAD_ARG;
 if(ctx->used>=64U||ctx->bytes>0x1fffffffffffffffULL)return RAF_LBR_BOUNDS;
 c=*ctx;bitlen=c.bytes*8ULL;
 c.block[c.used++]=0x80U;
 if(c.used>56U){
  for(i=c.used;i<64U;++i)c.block[i]=0;
  sha_block(&c);c.used=0U;
 }
 for(i=c.used;i<56U;++i)c.block[i]=0;
 for(i=0;i<8U;++i)c.block[56U+i]=(unsigned char)(bitlen>>(56U-8U*i));
 sha_block(&c);
 for(i=0;i<32U;++i)
  digest[i]=(unsigned char)(c.h[i/4U]>>(24U-8U*(i%4U)));
 return RAF_LBR_OK;
}
static int nonzero32(const unsigned char d[32]){
 u32 i,a=0U;for(i=0U;i<32U;++i)a|=(u32)d[i];return a!=0U;
}
static void append_u32(raf_lbr_sha256 *ctx,u32 x){
 unsigned char p[4];u32 i;
 for(i=0U;i<4U;++i)p[i]=(unsigned char)(x>>(24U-8U*i));
 (void)raf_lbr_sha256_update(ctx,p,4U);
}
static void append_u64(raf_lbr_sha256 *ctx,u64 x){
 unsigned char p[8];u32 i;
 for(i=0U;i<8U;++i)p[i]=(unsigned char)(x>>(56U-8U*i));
 (void)raf_lbr_sha256_update(ctx,p,8U);
}
int raf_lbr_batch_init(raf_lbr_batch *b){
 static const unsigned char domain[]="RAFAELIA.LIBRARIAN.BATCH.V1";
 if(!b)return RAF_LBR_BAD_ARG;
 raf_lbr_sha256_init(&b->stream);
 (void)raf_lbr_sha256_update(&b->stream,domain,(u32)(sizeof(domain)-1U));
 b->count=0U;b->links=0U;b->held=0U;b->unknown=0U;
 b->conversations=0U;b->messages=0U;b->candidates=0U;
 return RAF_LBR_OK;
}
int raf_lbr_batch_add(raf_lbr_batch *b,const raf_lbr_item *x){
 u32 i;
 if(!b||!x)return RAF_LBR_BAD_ARG;
 if(b->count>=64U)return RAF_LBR_BOUNDS;
 if(b->count && x->ordinal<=b->items[b->count-1U].ordinal)
  return RAF_LBR_ORDER;
 if(x->origin<RAF_LBR_DRIVE_ORIGIN||x->origin>RAF_LBR_UNKNOWN_ORIGIN||
    x->evidence<RAF_LBR_BYTES_VERIFIED||x->evidence>RAF_LBR_WITNESS_VERIFIED)
  return RAF_LBR_EVIDENCE;
 if(x->origin==RAF_LBR_DRIVE_WITNESS && x->evidence!=RAF_LBR_WITNESS_VERIFIED)
  return RAF_LBR_EVIDENCE;
 if(x->origin==RAF_LBR_DRIVE_ORIGIN && x->evidence==RAF_LBR_WITNESS_VERIFIED)
  return RAF_LBR_EVIDENCE;
 if(x->origin==RAF_LBR_UNKNOWN_ORIGIN && x->evidence!=RAF_LBR_METADATA_ONLY)
  return RAF_LBR_EVIDENCE;
 if(!nonzero32(x->locator_sha256))return RAF_LBR_EVIDENCE;
 if(x->evidence!=RAF_LBR_METADATA_ONLY &&
   (!x->size_bytes||!nonzero32(x->content_sha256)))
  return RAF_LBR_EVIDENCE;
 if(x->evidence==RAF_LBR_METADATA_ONLY &&
    (x->conversation_count||x->message_count))
  return RAF_LBR_EVIDENCE;
 if(x->conversation_count>0xffffffffU-b->conversations||
    x->message_count>0xffffffffU-b->messages)return RAF_LBR_OVERFLOW;
 b->items[b->count]=*x;
 append_u32(&b->stream,0x46494c45U); /* FILE tag */
 append_u32(&b->stream,x->ordinal);
 append_u32(&b->stream,x->origin);
 append_u32(&b->stream,x->evidence);
 append_u64(&b->stream,x->size_bytes);
 append_u32(&b->stream,x->conversation_count);
 append_u32(&b->stream,x->message_count);
 (void)raf_lbr_sha256_update(&b->stream,x->locator_sha256,32U);
 (void)raf_lbr_sha256_update(&b->stream,x->content_sha256,32U);
 ++b->count;b->conversations+=x->conversation_count;b->messages+=x->message_count;
 if(x->evidence!=RAF_LBR_BYTES_VERIFIED)++b->held;
 if(x->origin==RAF_LBR_UNKNOWN_ORIGIN)++b->unknown;
 for(i=0U;i<0U;++i){} /* reserve no global mutable state */
 return RAF_LBR_OK;
}
int raf_lbr_batch_link(raf_lbr_batch *b,u32 a,u32 z,u32 relation,u32 state){
 u32 i,ia=64U,iz=64U,j,diff=0U;
 if(!b)return RAF_LBR_BAD_ARG;
 if(a==z||relation<RAF_LBR_SAME_BYTES||relation>RAF_LBR_DERIVED_FROM||
    state<RAF_LBR_REL_OBSERVED||state>RAF_LBR_REL_TOKEN_VAZIO)
  return RAF_LBR_RELATION;
 if(b->links==0xffffffffU||b->candidates==0xffffffffU)
  return RAF_LBR_OVERFLOW;
 for(i=0U;i<b->count;++i){
  if(b->items[i].ordinal==a)ia=i;
  if(b->items[i].ordinal==z)iz=i;
 }
 if(ia==64U||iz==64U)return RAF_LBR_RELATION;
 if(relation==RAF_LBR_SAME_BYTES){
  if(state!=RAF_LBR_REL_OBSERVED||
     b->items[ia].evidence==RAF_LBR_METADATA_ONLY||
     b->items[iz].evidence==RAF_LBR_METADATA_ONLY||
     b->items[ia].size_bytes!=b->items[iz].size_bytes)
   return RAF_LBR_EVIDENCE;
  for(j=0U;j<32U;++j)
   diff|=(u32)(b->items[ia].content_sha256[j]^b->items[iz].content_sha256[j]);
  if(diff)return RAF_LBR_EVIDENCE;
 }else if(state==RAF_LBR_REL_OBSERVED)return RAF_LBR_EVIDENCE;
 append_u32(&b->stream,0x4c494e4bU); /* LINK tag */
 append_u32(&b->stream,a);append_u32(&b->stream,z);
 append_u32(&b->stream,relation);append_u32(&b->stream,state);
 ++b->links;
 if(state!=RAF_LBR_REL_OBSERVED)++b->candidates;
 return RAF_LBR_OK;
}
int raf_lbr_batch_finish(const raf_lbr_batch *b,raf_lbr_summary *out){
 raf_lbr_summary s;
 if(!b||!out)return RAF_LBR_BAD_ARG;
 if(raf_lbr_sha256_final(&b->stream,s.receipt_sha256)!=RAF_LBR_OK)
  return RAF_LBR_BOUNDS;
 s.count=b->count;s.links=b->links;s.held=b->held;
 s.unknown=b->unknown;s.conversations=b->conversations;
 s.messages=b->messages;s.candidates=b->candidates;
 s.promotion_allowed=0U;
 *out=s;return RAF_LBR_OK;
}
