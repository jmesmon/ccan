#include <ccan/ilog/ilog.h>
#include <ccan/ilog/ilog.c>
#include <stdio.h>
#include <ccan/tap/tap.h>

#include "test_ilogs.h"

#define NTRIALS (64)

int main(int _argc,const char *_argv[]){
  int i;
  int j;
  int (*il32)(uint32_t) = ilog_32;
  int (*il64)(uint64_t) = ilog_64;
  int (*il32_nz)(uint32_t) = ilog_32_nz;
  int (*il64_nz)(uint64_t) = ilog_64_nz;

  /*This is how many tests you plan to run.*/
  plan_tests(33 * NTRIALS * 3 + 65 * NTRIALS * 3);
  for(i=0;i<=32;i++){
    uint32_t v;
    /*Test each bit in turn (and 0).*/
    v=i?(uint32_t)1U<<(i-1):0;
    for(j=0;j<NTRIALS;j++){
      int l;
      l=test_ilog_32(v);
      ok1(STATIC_ILOG_32(v)==l);
      ok1(il32(v)==l);
      ok1(il32_nz(v) == l || v == 0);
      /*Also try a few more pseudo-random values with at most the same number
         of bits.*/
      v=pseudo_rand_32(j);
    }
  }

  for(i=0;i<=64;i++){
    uint64_t v;
    /*Test each bit in turn (and 0).*/
    v=i?(uint64_t)1U<<(i-1):0;
    for(j=0;j<NTRIALS;j++){
      int l;
      l=test_ilog_64(v);
      ok1(STATIC_ILOG_64(v)==l);
      ok1(il64(v)==l);
      ok1(il64_nz(v) == l || v == 0);
      /*Also try a few more pseudo-random values with at most the same number
         of bits.*/
      v=pseudo_rand_64(j);
    }
  }
  return exit_status();
}
