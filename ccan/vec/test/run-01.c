#include <ccan/vec/vec.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

int main(void)
{

    int v[] = {2,5,6,7,11};
    int i;
    plan_tests(ARRAY_SIZE(v) + 4);
    VEC(int) nums = VEC_INIT;

    for (i = 0; i < ARRAY_SIZE(v); i++)
        vec_append(&nums, v[i]);

    ok1(vec_len(&nums) == ARRAY_SIZE(v));

    int j = 0, *p;
    vec_for_each_val(&nums, i, p) {
        ok1(v[j] == i);
        j ++;
    }

    VEC(int) nums2 = VEC_INIT;
    vec_append(&nums2, 4);
    vec_append(&nums2, 1);

    vec_concat(&nums, &nums2);

    ok1(vec_len(&nums) == (ARRAY_SIZE(v) + 2));
    ok1(*vec_index(&nums, vec_len(&nums) - 1) == 1);
    ok1(*vec_index(&nums, vec_len(&nums) - 2) == 4);

    vec_drop(&nums2);
    vec_drop(&nums);

    return exit_status();
}
