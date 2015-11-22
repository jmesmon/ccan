#include "../bheapm.h"

#include <ccan/container_of/container_of.h>
#include <ccan/compiler/compiler.h>
#include <ccan/order/order.h>

#include <stdio.h>

struct X {
	struct bheapm_node bhn;
	int v;
};

#define bhn_to_X(b) container_of(b, struct X, bhn)

static UNNEEDED
int X_cmp_bh(const struct bheapm_ *bh, const struct bheapm_node *a, const struct bheapm_node *b)
{
	(void)bh;
	return bhn_to_X(a)->v - bhn_to_X(b)->v;
}

#define X_BHEAPM(name_) \
	BHEAPM(struct X, bhn) name_ = BHEAPM_INIT(X_cmp_bh)

static UNNEEDED
int X_to_string(struct bheapm_node *b, char *str, size_t l)
{
	struct X *x = container_of(b, struct X, bhn);
	return snprintf(str, l, "%d", x->v);
}

static UNNEEDED
int X_cmp(const struct X *a, const struct X *b)
{
	return a->v - b->v;
}
