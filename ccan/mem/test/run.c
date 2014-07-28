#include <ccan/array_size/array_size.h>
#include <ccan/mem/mem.h>
#include <ccan/tap/tap.h>

int main(void)
{
	char a[] = "hi\0bye";
	char b[] = "by";
	char c[] = "e\0";

#define A(x) x, sizeof(x)
#define AM1(x) x, (sizeof(x) - 1)
#define L(...) A((char []){ __VA_ARGS__ })
#define S(...) AM1((char []){ __VA_ARGS__})

	plan_tests(5);

	ok1(mempbrk(A(a), L("by")) == &a[3]);
	ok1(mempbrk(A(b), L("\0y")) == &b[1]);
	ok1(mempbrk(A(b), L("FOO")) == (b + strlen(b)));
	ok1(mempbrk(A(b), S("FOO")) == NULL);

	ok1(mempbrk_str(A(a), "by") == &a[3]);

	return exit_status();
}
