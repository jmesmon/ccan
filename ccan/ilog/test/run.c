#include <ccan/ilog/ilog.h>
#include <ccan/ilog/ilog.c>
#include <stdio.h>
#include <ccan/tap/tap.h>

#include "test_ilogs.h"

#define NTRIALS (64)
#define do_it(bits)								\
  do {										\
  for (i=0;i<=bits;i++) {							\
    uint##bits##_t v;								\
    /*Test each bit in turn (and 0).*/						\
    v=i?(uint##bits##_t)1U<<(i-1):0;						\
    for(j=0;j<NTRIALS;j++){							\
      int l;									\
      l=test_ilog_##bits(v);							\
      ok1(STATIC_ILOG_##bits(v)==l);						\
      ok1(ilog_##bits(v)==l);							\
      ok1(ilog_##bits##_nz(v) == l || v == 0);					\
      typedef u8_ret_t (*ilog_##bits##_fn)(uint##bits##_t);			\
      ilog_##bits##_fn ilog_##bits##_non_builtin = ilog_##bits;			\
      ok1(ilog_##bits##_non_builtin(v) == l);					\
      /* Also try a few more pseudo-random values with at most the same */	\
      /* number of bits. */							\
      v=pseudo_rand_##bits(i);							\
    }										\
  } } while(0)

#define ct_test(bits) ((bits+1)*NTRIALS*4)

int main(int _argc,const char *_argv[]){
  int i;
  int j;
  /* This is how many tests you plan to run */
  plan_tests(ct_test(32) + ct_test(64));
  do_it(32);
  do_it(64);
  return exit_status();
}
