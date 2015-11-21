#include <ccan/container_of/container_of.h>
#include <ccan/bheap/bheap.h>
#include <ccan/compiler/compiler.h>
#include <ccan/order/order.h>

#include <stdio.h>

struct X {
	struct bheap_node bhn;
	int v;
};

#define bhn_to_X(b) container_of(b, struct X, bhn)

#define X_BHEAP(name_) \
	total_order_by_field(name_##__X_ord, int, struct X, v); \
	BHEAP(struct X, bhn) name_ = BHEAP_INIT(&name_, name_##__X_ord.cb, name_##__X_ord.ctx)

static UNNEEDED
int X_to_string(struct bheap_node *b, char *str, size_t l)
{
	struct X *x = container_of(b, struct X, bhn);
	return snprintf(str, l, "%d", x->v);
}

static UNNEEDED
int X_cmp(const struct X *a, const struct X *b)
{
	return a->v - b->v;
}
