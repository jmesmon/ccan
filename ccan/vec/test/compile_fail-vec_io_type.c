#include <ccan/vec/vec_io.h>

int main(void)
{
    int r;
#ifdef FAIL
    VEC(int) vec = VEC_INIT;
#else
    VEC(char) vec = VEC_INIT;
#endif
    r = vec_printf(&vec, "{");
    assert(r >= 0);
    vec_drop(&vec);
    return 0;
}
