/* Licenced under LGPLv3+ - see LICENSE file for details */
#ifndef CCAN_BHEAPM_H_
#define CCAN_BHEAPM_H_

#include "config.h"

#include <stddef.h>
#include <stdint.h>

#include <ccan/compiler/compiler.h>
#include <ccan/ilog/ilog.h>
#include <ccan/order/order.h>
#include <ccan/tcon/tcon.h>

/**
 * struct bheapm_node - add this to your structures to be inserted in the heap
 *
 * Example:
 * struct foo {
 *	int x;
 *	struct bheapm_node bn;
 * };
 */
struct bheapm_node {
	struct bheapm_node *d[2];
};

struct bheapm_;

/*
 * bheapm_order_fn - provide a total ordering for the bheapm_nodes in the heap
 * @bh: the unwrapped bheapm, provided so auxillary data for ordering can be located
 * @x: "left" node to comare
 * @y: "right" note to compare
 *
 * Generally, ordering will look like "x - y", for some definition of '-'.
 *
 * Returns 0 when nodes are equal
 * Returns >0 when x > y
 * Returns <0 when x < y
 *
 * See also:
 *	BHEAPM_INIT(), bheapm_init()
 */
typedef int (*bheapm_order_fn)(const struct bheapm_ *bh,
		const struct bheapm_node *x, const struct bheapm_node *y);

/**
 * struct bheapm_ - the heap itself
 * @sz: number of items contained in the heap
 * @top: the minimum node
 * @order: ordering callback & ctx
 *
 * Each heap has a single ordering function, and tracks a set of nodes.
 */
struct bheapm_ {
	bheapm_order_fn order;
	struct bheapm_node *top;
	size_t sz;
};

/**
 * BHEAPM - defines, but doesn't initialize a binary heap
 * @type: type of items in the queue / items compared by the callback
 * @node: name of the bheapm_node in @type
 *
 * See also:
 *	bheapm_init(), BHEAPM_INIT()
 */
#define BHEAPM(type_, node_) \
	TCON_WRAP(struct bheapm_, \
		TCON_CONTAINER(canary, type_, node_))

/**
 * BHEAPM_INIT - initializer for a 'struct bheapm_'
 * @bh_: bheapm we're the initializer for (we use this to do type checking)
 * @order_cb_: ordering callback
 * @order_ctx_: order context
 *
 * Example:
 * #include <ccan/container_of/container_of.h>
 * static int cmp_foo(const struct bheapm_ *bh, const struct bheapm_node *a, const struct bheapm_node *b) {
 *	(void)bh;
 *	return container_of(a, struct foo, bn)->x - container_of(b, struct foo, bn)->x;
 * }
 * BHEAPM(struct foo, bn) some_heap = BHEAPM_INIT(cmp_foo);
 *
 */
#define BHEAPM_INIT(order_cb_) TCON_WRAP_INIT({	\
	.order = order_cb_ \
})

/**
 * bheapm_init - initialize a new binary heap
 * @b: a new binary heap
 * @ord: an ordering function
 *
 * Always succeeds
 *
 * Example:
 * ...
 *      BHEAPM(struct foo, bn) bh;
 *	bheapm_init(&bh, cmp_foo);
 */
#define bheapm_init(bh_, order_cb_) \
	bheapm_init_(tcon_unwrap(bh_), (order_cb_))
static inline void bheapm_init_(struct bheapm_ *b, bheapm_order_fn order_cb)
{
	b->order = order_cb;
	b->sz = 0;
	b->top = NULL;
}

/**
 * bheapm_entry - convert a bheapm_node back to the structure containing it
 * @b: (struct bheapm_ *), the containing bheap
 * @node: (struct bheapm_node *), the contained node
 */
#define bheapm_entry(bh_, node_) \
	tcon_container_of((bh_), canary, (node_))

/**
 * bheapm_push - add a node to a binary heap
 * @b: the heap to add to
 * @new_node: the node to add
 *
 * Example:
 * ...
 *	struct foo x[] = { {1}, {2} };
 *	bheapm_push(&bh, &x[0]);
 *	bheapm_push(&bh, &x[1]);
 */
#define bheapm_push(bh_, new_node_) \
	bheapm_push_(tcon_unwrap(bh_), \
		tcon_member_of((bh_), canary, (new_node_)))
void bheapm_push_(struct bheapm_ *b, struct bheapm_node *new_node);

/**
 * bheapm_pop - remove and return the lowest node in the binary heap
 * @bh: the heap to remove from
 *
 * Returns a node that was removed from @b, or NULL if @b was empty.
 *
 * Example:
 * #include <stdio.h>
 * ...
 *	struct foo *lowest = bheapm_pop(&bh);
 *	printf("lowest = %d\n", lowest->x);
 *
 */
#define bheapm_pop(bh_) \
	bheapm_entry((bh_), bheapm_pop_(tcon_unwrap(bh_)))
struct bheapm_node *bheapm_pop_(struct bheapm_ *bh);

/**
 * bheapm_peek - get the next element that would be removed from the heap
 * @bh: the heap to look at a noce from
 *
 */
#define bheapm_peek(bh_) \
	bheapm_entry((bh_), bheapm_peek_(tcon_unwrap(bh_)))
static inline struct bheapm_node *bheapm_peek_(const struct bheapm_ *bh)
{
	return bh->top;
}

/**
 * bheapm_depth - given a number of nodes, return the depth of the heap
 * @n: non-zero number of nodes
 *
 */
static inline uint_fast8_t bheapm_depth(size_t n)
{
	return ilog64_nz(n);
}

/* -- Internal --
 *
 * The code does some aliasing between bheapm and bheapm_node to
 * access the bheapm_node pointers they both contain.
 *
 * C requires this union exist for that to be allowed.
 */
union bheapm_union {
	struct bheapm_ heap;
	struct bheapm_node  node;
	struct bheapm_node *node_p;
};

#endif
