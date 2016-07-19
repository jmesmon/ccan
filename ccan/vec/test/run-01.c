#include <ccan/vec/vec.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>

int main(void)
{

    int v[] = {2,5,6,7,11};
    int i;
    plan_tests(ARRAY_SIZE(v) * 3 + 4 + 4);
    VEC(int) nums = VEC_INIT;

    for (i = 0; i < ARRAY_SIZE(v); i++) {
#if HAVE_TYPEOF
        ok1(vec_append(&nums, v[i]) >= 0);
#else
        skip(1, "lazy, should be checked before assingment");
        *(int *)vec_append_space(&nums) = v[i];
#endif
    }


    ok1(vec_len(&nums) == ARRAY_SIZE(v));

    int *p;
    size_t j = 0;
    vec_for_each_val(&nums, i, p) {
        ok1(v[j] == i);
        j ++;
    }

    VEC(int) nums2 = VEC_INIT;
#if HAVE_TYPEOF
    ok1(vec_append(&nums2, 4) >= 0);
    ok1(vec_append(&nums2, 1) >= 0);
#else
    skip(2, "I'm lazy");
    *(int *)vec_append_space(&nums2) = 4;
    *(int *)vec_append_space(&nums2) = 1;
#endif

    vec_concat(&nums, &nums2);

    ok1(vec_len(&nums) == (ARRAY_SIZE(v) + 2));
#if HAVE_TYPEOF
    ok1(*vec_index(&nums, vec_len(&nums) - 1) == 1);
    ok1(*vec_index(&nums, vec_len(&nums) - 2) == 4);
#else
    ok1(*(int *)vec_index(&nums, vec_len(&nums) - 1) == 1);
    ok1(*(int *)vec_index(&nums, vec_len(&nums) - 2) == 4);
#endif

    j = 0;
    vec_for_each(&nums, p) {
        ok1(vec_offset(&nums, p) == j);
        j++;
    }

    vec_drop(&nums2);
    vec_drop(&nums);

    return exit_status();
}
