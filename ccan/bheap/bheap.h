/* Licenced under LGPLv3+ - see LICENSE file for details */
#ifndef CCAN_BHEAP_H_
#define CCAN_BHEAP_H_

#include <stddef.h>
#include <ccan/compiler/compiler.h>

struct binary_heap_node {
	struct binary_heap_node *d[2];
};

/**
 * bheap_ordering_fn - a function that defines an order over embedded binary heap nodes.
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
 * struct foo {
 *	int x;
 *	struct binary_heap_node b;
 * };
 * // this is a bheap_ordering_fn
 * static int ord_foo(const struct binary_heap_node *a,
 *		      const struct binary_heap_node *b)
 * {
 *	return container_of(a, struct foo, b)->x
 *		- container_of(b, struct foo, b)->x;
 * }
 *
 */
typedef int (*bheap_ordering_fn)(const struct binary_heap_node *a,
				 const struct binary_heap_node *b);

struct binary_heap {
	struct binary_heap_node *top;
	size_t sz;

	/* TODO: This isn't ideal: function pointers are slow */
	bheap_ordering_fn ord;
};

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
 *	struct binary_heap b;
 *	bheap_init(&b, ord_foo);
 */
void bheap_init(struct binary_heap *b, bheap_ordering_fn ord);

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
void bheap_push(struct binary_heap *b, struct binary_heap_node *new_node);

/**
 * bheap_pop - remove and return the lowest node in the binary heap
 * @b: the heap to remove from
 *
 * Returns a node that was removed from @b, or NULL if @b was empty.
 *
 * Example:
 * #include <stdio.h>
 * ...
 *	struct binary_heap_node *v = bheap_pop(&b);
 *	struct foo *lowest = container_of(v, struct foo, b);
 *	printf("lowest = %d\n", lowest->x);
 *
 */
struct binary_heap_node *bheap_pop(struct binary_heap *b);

/* -- Internal --
 *
 * The code does some aliasing between binary_heap and binary_heap_node to
 * access the binary_heap_node pointers they both contain.
 *
 * C requires this union exist for that to be allowed.
 */
union binary_heap_union {
	struct binary_heap heap;
	struct binary_heap_node  node;
	struct binary_heap_node *node_p;
};

#endif
