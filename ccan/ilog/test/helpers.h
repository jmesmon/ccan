
/*Dead simple (but slow) versions to compare against.*/
static int test_ilog32(uint32_t _v)
{
  int ret;
  for(ret=0;_v;ret++)_v>>=1;
  return ret;
}

static int test_ilog64(uint64_t _v)
{
  int ret;
  for(ret=0;_v;ret++)_v>>=1;
  return ret;
}

static uint64_t pseudo_rand_64(uint64_t v, int i)
{
  return ((uint64_t)((2862933555777941757ULL*v+3037000493ULL)
    &0xFFFFFFFFFFFFFFFFULL>>((65-i)>>1)>>((64-i)>>1)));
}

static uint32_t pseudo_rand_32(uint64_t v, int i)
{
  return ((1103515245U*v+12345U)&0xFFFFFFFFU>>((33-i)>>1)>>((32-i)>>1));
}

