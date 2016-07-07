#include <ccan/tlist2/tlist2.h>

struct child {
	const char *name;
	struct list_node list;
};

struct cousin {
	const char *name;
	struct list_node list;
};

int main(int argc, char *argv[])
{
	TLIST2(struct child, list) children;
	struct child child = { "child" };
#ifdef FAIL
	struct cousin *p;
#else
	struct child *p;
#endif

	tlist2_init(&children);
	tlist2_add(&children, &child);
	p = tlist2_next(&children, &child);
	(void) p;
	return 0;
}
