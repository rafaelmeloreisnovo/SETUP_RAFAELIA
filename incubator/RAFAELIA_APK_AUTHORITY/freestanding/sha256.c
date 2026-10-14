/* RAFAELIA SHA-256 v1: freestanding digest primitive only.
 * NOT an authenticated signature, an APK verifier, or proof of authorship.
 */
typedef unsigned int rf_u32;
typedef unsigned long long rf_u64;
typedef unsigned char rf_u8;
_Static_assert(sizeof(rf_u32)==4, "uint32");
_Static_assert(sizeof(rf_u64)==8, "uint64");
_Static_assert(sizeof(rf_u8)==1, "byte");
typedef struct {
  rf_u32 h[8];
  rf_u64 total;
  rf_u32 used;
  rf_u32 stage;
  rf_u8 block[64];
} rf_sha256_ctx;
static const rf_u32 k[64]={
0x428a2f98U,0x71374491U,0xb5c0fbcfU,0xe9b5dba5U,0x3956c25bU,0x59f111f1U,0x923f82a4U,0xab1c5ed5U,
0xd807aa98U,0x12835b01U,0x243185beU,0x550c7dc3U,0x72be5d74U,0x80deb1feU,0x9bdc06a7U,0xc19bf174U,
0xe49b69c1U,0xefbe4786U,0x0fc19dc6U,0x240ca1ccU,0x2de92c6fU,0x4a7484aaU,0x5cb0a9dcU,0x76f988daU,
0x983e5152U,0xa831c66dU,0xb00327c8U,0xbf597fc7U,0xc6e00bf3U,0xd5a79147U,0x06ca6351U,0x14292967U,
0x27b70a85U,0x2e1b2138U,0x4d2c6dfcU,0x53380d13U,0x650a7354U,0x766a0abbU,0x81c2c92eU,0x92722c85U,
0xa2bfe8a1U,0xa81a664bU,0xc24b8b70U,0xc76c51a3U,0xd192e819U,0xd6990624U,0xf40e3585U,0x106aa070U,
0x19a4c116U,0x1e376c08U,0x2748774cU,0x34b0bcb5U,0x391c0cb3U,0x4ed8aa4aU,0x5b9cca4fU,0x682e6ff3U,
0x748f82eeU,0x78a5636fU,0x84c87814U,0x8cc70208U,0x90befffaU,0xa4506cebU,0xbef9a3f7U,0xc67178f2U
};
static rf_u32 rotr(rf_u32 x,rf_u32 n) { return (x>>n)|(x<<(32U-n)); }
static void compress(rf_sha256_ctx *ctx) {
 rf_u32 w[64],a,b,c,d,e,f,g,h,t1,t2,i;
 for(i=0;i<16U;++i) {
   rf_u32 j=4U*i;
   w[i]=((rf_u32)ctx->block[j]<<24U)|((rf_u32)ctx->block[j+1U]<<16U)|
        ((rf_u32)ctx->block[j+2U]<<8U)|ctx->block[j+3U];
 }
 for(i=16U;i<64U;++i) {
  rf_u32 x=w[i-15U],y=w[i-2U];
  rf_u32 s0=rotr(x,7U)^rotr(x,18U)^(x>>3U);
  rf_u32 s1=rotr(y,17U)^rotr(y,19U)^(y>>10U);
  w[i]=w[i-16U]+s0+w[i-7U]+s1;
 }
 a=ctx->h[0];b=ctx->h[1];c=ctx->h[2];d=ctx->h[3];
 e=ctx->h[4];f=ctx->h[5];g=ctx->h[6];h=ctx->h[7];
 for(i=0;i<64U;++i) {
  rf_u32 s1=rotr(e,6U)^rotr(e,11U)^rotr(e,25U);
  rf_u32 ch=(e&f)^((~e)&g);
  rf_u32 s0=rotr(a,2U)^rotr(a,13U)^rotr(a,22U);
  rf_u32 maj=(a&b)^(a&c)^(b&c);
  t1=h+s1+ch+k[i]+w[i];t2=s0+maj;
  h=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;
 }
 ctx->h[0]+=a;ctx->h[1]+=b;ctx->h[2]+=c;ctx->h[3]+=d;
 ctx->h[4]+=e;ctx->h[5]+=f;ctx->h[6]+=g;ctx->h[7]+=h;
}
int rf_sha256_init(rf_sha256_ctx *ctx) {
 static const rf_u32 iv[8]={
 0x6a09e667U,0xbb67ae85U,0x3c6ef372U,0xa54ff53aU,
 0x510e527fU,0x9b05688cU,0x1f83d9abU,0x5be0cd19U
 };
 rf_u32 i;
 if(!ctx)return 0;
 for(i=0;i<8U;++i)ctx->h[i]=iv[i];
 ctx->total=0ULL;ctx->used=0U;ctx->stage=1U;
 for(i=0;i<64U;++i)ctx->block[i]=0U;
 return 1;
}
int rf_sha256_update(rf_sha256_ctx *ctx,const rf_u8 *data,rf_u64 length) {
 if(!ctx||ctx->stage!=1U||(length!=0ULL&&!data))return 0;
 if(ctx->total>0x1fffffffffffffffULL||
    length>0x1fffffffffffffffULL-ctx->total)return 0;
 ctx->total+=length;
 while(length!=0ULL) {
  ctx->block[ctx->used++]=*data++;--length;
  if(ctx->used==64U){compress(ctx);ctx->used=0U;}
 }
 return 1;
}
int rf_sha256_final(rf_sha256_ctx *ctx,rf_u8 *out) {
 rf_u64 bits;
 rf_u32 i;
 if(!ctx||!out||ctx->stage!=1U||ctx->used>63U)return 0;
 bits=ctx->total<<3U;
 ctx->block[ctx->used++]=0x80U;
 if(ctx->used>56U){
  while(ctx->used<64U)ctx->block[ctx->used++]=0U;
  compress(ctx);ctx->used=0U;
 }
 while(ctx->used<56U)ctx->block[ctx->used++]=0U;
 for(i=0;i<8U;++i)ctx->block[56U+i]=(rf_u8)(bits>>(56U-8U*i));
 compress(ctx);
 for(i=0;i<8U;++i){
  out[4U*i]=(rf_u8)(ctx->h[i]>>24U);
  out[4U*i+1U]=(rf_u8)(ctx->h[i]>>16U);
  out[4U*i+2U]=(rf_u8)(ctx->h[i]>>8U);
  out[4U*i+3U]=(rf_u8)ctx->h[i];
 }
 ctx->stage=2U;ctx->used=0U;
 for(i=0;i<64U;++i)ctx->block[i]=0U;
 return 1;
}
