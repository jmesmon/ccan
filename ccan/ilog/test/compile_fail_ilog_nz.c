#include <ccan/tap/tap.h>
#include <ccan/ilog/ilog.h>

int main(void) {
#ifdef FAIL
   int f = ilog_nz(1.0);
#else
   int f = 0;
#endif
   return f;
}
