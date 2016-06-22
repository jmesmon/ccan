#include <ccan/vec/vec_io.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>
#include <string.h>
#include <ccan/str/str.h>

int main(void)
{
    int r;
    VEC(char) vec = VEC_INIT;
    plan_tests(4);

    r = vec_printf(&vec, "{");
    ok1(r == strlen("{"));
    ok1(streq(vec_data(&vec), "{"));

    unsigned long tv_sec = 1465919676;
    printf("sec = %lu\n", tv_sec);
    r = vec_printf(&vec, "'dt':%lu", tv_sec);
    ok1(r > 0);

    double m = 1.353;
    r = vec_printf(&vec, ",'dc':{'u':1,'v':%lf}", m);
    ok1(r > 0);

    vec_drop(&vec);

    return exit_status();
}
