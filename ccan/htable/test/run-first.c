#include <ccan/htable/htable.h>
#include <ccan/htable/htable.c>
#include <ccan/tap/tap.h>

#define NUM_VALS 512

static size_t hash(const void *elem, void *unused)
{
	size_t h = *(uint64_t *)elem / 2;
	return h;
}

int main(int argc, char *argv[])
{
	struct htable ht;
	struct htable_iter iter;

	plan_tests(1);
	htable_init(&ht, hash, NULL);
	ok1(!htable_first(&ht, &iter));
	htable_clear(&ht);
	return exit_status();
}
