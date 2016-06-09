#include <ccan/vec/vec.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

int main(void)
{

    int v[] = {2,5,6,7,11};
    int i;
    plan_tests(ARRAY_SIZE(v) + 1);
    VEC(int) nums = VEC_INIT;

    for (i = 0; i < ARRAY_SIZE(v); i++)
        vec_append(&nums, v[i]);

    ok1(vec_len(&nums) == ARRAY_SIZE(v));

    int j = 0, *p;
    vec_for_each_val(&nums, i, p) {
        ok1(v[j] == i);
        j ++;
    }

    vec_drop(&nums);

    return exit_status();
}
