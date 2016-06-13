#include <ccan/vec/vec_io.h>
#include <ccan/tap/tap.h>
#include <ccan/array_size/array_size.h>
#include <string.h>
#include <ccan/str/str.h>

int main(void)
{
    int r;
    VEC(char) str = VEC_INIT;
    plan_tests(4);

    r = vec_printf(&str, "hello");
    ok1(r == strlen("hello"));
    ok1(streq(vec_data(&str), "hello"));

    r = vec_printf(&str, "bye");
    ok1(r == strlen("bye"));

    ok1(streq(vec_data(&str), "hellobye"));

    vec_drop(&str);

    return exit_status();
}
