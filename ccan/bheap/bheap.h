/* Licenced under LGPLv3+ - see LICENSE file for details */
#ifndef CCAN_BHEAP_H_
#define CCAN_BHEAP_H_

#include "config.h"
#include <ccan/compiler/compiler.h>
#include <ccan/ilog/ilog.h>

#include <stddef.h>
#include <stdint.h>

/**
 * struct bheap_node - add this to your structures to be inserted in the heap
 *
 * Example:
 * struct foo {
 *	int x;
 *	struct bheap_node b;
 * };
 */
struct bheap_node {
	struct bheap_node *d[2];
};

/**
 * bheap_ordering_fn - a function that defines an order over heap nodes.
 * @a: one node
 * @b: another node
 *
 * IF @a < @b, return < 0
 * IF @a > @b, return > 0
 * IF @a = @b, return = 0
 *
 * The heap will be ordered with the "lowest" node on top.
 * For a max-heap, simply invert the ord function.
 *
 * In 'ord(a,b) > 0', this behaves like '@a > @b' (greater than)
 * In 'ord(a,b) < 0', this behaves like '@a < @b' (less than)
 *
 * And similarly with equals added.
 *
 * The operation could be "A - B", for some definition of "-"
 * (minus/difference)
 *
 * Example:
 * #include <ccan/container_of/container_of.h>
 * // this is a bheap_ordering_fn
 * static int ord_foo(const struct bheap_node *a,
 *		      const struct bheap_node *b)
 * {
 *	return container_of(a, struct foo, b)->x
 *		- container_of(b, struct foo, b)->x;
 * }
 *
 */
typedef int (*bheap_ordering_fn)(const struct bheap_node *a,
				 const struct bheap_node *b);

/**
 * struct bheap - the heap itself
 *
 * Each heap has a single ordering function, and tracks a set of nodes.
 *
 * Example:
 * struct bheap b;
 */
struct bheap {
	struct bheap_node *top;
	size_t sz;

	/* TODO: This isn't ideal: function pointers are slow */
	bheap_ordering_fn ord;
};

/**
 * BHEAP_INIT - initializer for a 'struct bheap'
 * @ord_fn: a bheap_ordering_fn to use for this heap
 *
 * Example:
 * struct bheap some_heap = BHEAP_INIT(ord_foo);
 *
 */
#define BHEAP_INIT(ord_fn) {	\
		.ord = ord_fn	\
	}			\

/**
 * bheap_init - initialize a new binary heap
 * @b: a new binary heap
 * @ord: an ordering function
 *
 * Always succeeds
 *
 * Example:
 * ...
 *	bheap_init(&b, ord_foo);
 */
void bheap_init(struct bheap *b, bheap_ordering_fn ord);

/**
 * bheap_push - add a node to a binary heap
 * @b: the heap to add to
 * @new_node: the node to add
 *
 * Example:
 * ...
 *	struct foo x[] = { {1}, {2} };
 *	bheap_push(&b, &x[0].b);
 *	bheap_push(&b, &x[1].b);
 */
void bheap_push(struct bheap *b, struct bheap_node *new_node);

/**
 * bheap_pop - remove and return the lowest node in the binary heap
 * @b: the heap to remove from
 *
 * Returns a node that was removed from @b, or NULL if @b was empty.
 *
 * Example:
 * #include <stdio.h>
 * ...
 *	struct bheap_node *v = bheap_pop(&b);
 *	struct foo *lowest = container_of(v, struct foo, b);
 *	printf("lowest = %d\n", lowest->x);
 *
 */
struct bheap_node *bheap_pop(struct bheap *b);

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
	struct bheap heap;
	struct bheap_node  node;
	struct bheap_node *node_p;
};

#endif
