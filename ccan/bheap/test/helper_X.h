#include <ccan/container_of/container_of.h>
#include <ccan/bheap/bheap.h>
#include <ccan/compiler/compiler.h>

#include <stdio.h>

struct X {
	struct binary_heap_node bhn;
	int v;
};

#define bhn_to_X(b) container_of(b, struct X, bhn)

static UNNEEDED
int X_ord(const struct binary_heap_node *a, const struct binary_heap_node *b)
{
	const struct X *A = container_of(a, struct X, bhn), *B = container_of(b, struct X, bhn);

	return A->v - B->v;
}

static UNNEEDED
int X_to_string(struct binary_heap_node *b, char *str, size_t l)
{
	struct X *x = container_of(b, struct X, bhn);
	return snprintf(str, l, "%d", x->v);
}

static UNNEEDED
int X_cmp(const struct X *a, const struct X *b)
{
	return a->v - b->v;
}
