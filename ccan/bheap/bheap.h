/* Licenced under LGPLv3+ - see LICENSE file for details */
#ifndef CCAN_BHEAP_H_
#define CCAN_BHEAP_H_

#include "config.h"

#include <stddef.h>
#include <stdint.h>

#include <ccan/compiler/compiler.h>
#include <ccan/ilog/ilog.h>
#include <ccan/order/order.h>
#include <ccan/tcon/tcon.h>

/**
 * struct bheap_node - add this to your structures to be inserted in the heap
 *
 * Example:
 * struct foo {
 *	int x;
 *	struct bheap_node bn;
 * };
 */
struct bheap_node {
	struct bheap_node *d[2];
};

/**
 * struct bheap_ - the heap itself
 * @sz: number of items contained in the heap
 * @top: the minimum node
 * @order: ordering callback & ctx
 *
 * Each heap has a single ordering function, and tracks a set of nodes.
 */
struct bheap_ {
	struct _total_order order;
	struct bheap_node *top;
	size_t sz;
};

/**
 * BHEAP - defines, but doesn't initialize a binary heap
 * @type: type of items in the queue / items compared by the callback
 * @node: name of the bheap_node in @type
 *
 * See also:
 *	bheap_init(), BHEAP_INIT()
 */
#define BHEAP(type_, node_) \
	TCON_WRAP(struct bheap_, \
		TCON_CONTAINER(canary, type_, node_))

/**
 * BHEAP_INIT - initializer for a 'struct bheap_'
 * @bh_: bheap we're the initializer for (we use this to do type checking)
 * @order_cb_: ordering callback
 * @order_ctx_: order context
 *
 * Example:
 * #include <ccan/order/order.h>
 * struct foo2 {
 *	int x;
 *	struct bheap_node bn;
 * };
 * static int cmp_foo(const struct foo2 *a, const struct foo2 *b, void *ctx) {
 *	(void)ctx;
 *	return a->x - b->x;
 * }
 *
 * ...
 *	BHEAP(struct foo2, bn) some_heap = BHEAP_INIT(&some_heap, cmp_foo, NULL);
 *
 */
#define BHEAP_INIT(bh_, order_cb_, order_ctx_) TCON_WRAP_INIT({	\
	.order = { .cb = total_order_cast((order_cb_), \
		tcon_container_type((bh_), canary), \
		(order_ctx_)), \
		.ctx = (order_ctx_) \
	} \
})

/**
 * bheap_init - initialize a new binary heap
 * @b: a new binary heap
 * @ord: an ordering function
 *
 * Always succeeds
 *
 * Example:
 *      BHEAP(struct foo, bn) bh;
 *	total_order_by_field(my_order, int, struct foo, x);
 *	bheap_init(&bh, my_order.cb, my_order.ctx);
 */
#define bheap_init(bh_, order_cb_, order_ctx_) \
	bheap_init_(tcon_unwrap(bh_), \
		total_order_cast((order_cb_), \
			tcon_container_type((bh_), canary), \
			(order_ctx_)), \
		(order_ctx_))
static inline void bheap_init_(struct bheap_ *b, _total_order_cb order_cb, void *order_ctx)
{
	b->order.cb = order_cb;
	b->order.ctx = order_ctx;
	b->sz = 0;
	b->top = NULL;
}

/**
 * bheap_entry - convert a bheap_node back to the structure containing it
 * @b: (struct bheap_ *), the containing bheap
 * @node: (struct bheap_node *), the contained node
 */
#define bheap_entry(bh_, node_) \
	tcon_container_of((bh_), canary, (node_))

/**
 * bheap_push - add a node to a binary heap
 * @b: the heap to add to
 * @new_node: the node to add
 *
 * Example:
 * ...
 *	struct foo x[] = { {1}, {2} };
 *	bheap_push(&bh, &x[0]);
 *	bheap_push(&bh, &x[1]);
 */
#define bheap_push(bh_, new_node_) \
	bheap_push_(tcon_unwrap(bh_), \
		tcon_member_of((bh_), canary, (new_node_)), \
		tcon_offset((bh_), canary))
void bheap_push_(struct bheap_ *b, struct bheap_node *new_node, size_t offset);

/**
 * bheap_pop - remove and return the lowest node in the binary heap
 * @bh: the heap to remove from
 *
 * Returns a node that was removed from @b, or NULL if @b was empty.
 *
 * Example:
 * #include <stdio.h>
 * ...
 *	struct foo *lowest = bheap_pop(&bh);
 *	printf("lowest = %d\n", lowest->x);
 *
 */
#define bheap_pop(bh_) \
	bheap_entry((bh_), bheap_pop_(tcon_unwrap(bh_), \
			tcon_offset((bh_), canary)))
struct bheap_node *bheap_pop_(struct bheap_ *bh, size_t offset);

/**
 * bheap_peek - get the next element that would be removed from the heap
 * @bh: the heap to look at a noce from
 *
 */
#define bheap_peek(bh_) \
	bheap_entry((bh_), bheap_peek_(tcon_unwrap(bh_)))
static inline struct bheap_node *bheap_peek_(const struct bheap_ *bh)
{
	return bh->top;
}

/**
 * bheap_depth - given a number of nodes, return the depth of the heap
 * @n: non-zero number of nodes
 *
 */
static inline uint_fast8_t bheap_depth(size_t n)
{
	return ilog64_nz(n);
}

/* -- Internal --
 *
 * The code does some aliasing between bheap and bheap_node to
 * access the bheap_node pointers they both contain.
 *
 * C requires this union exist for that to be allowed.
 */
union bheap_union {
	struct bheap_ heap;
	struct bheap_node  node;
	struct bheap_node *node_p;
};

#endif
