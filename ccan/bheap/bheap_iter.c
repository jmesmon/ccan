/* Licenced under LGPLv3+ - see LICENSE file for details */
#include "bheap.h"
#include <assert.h>
#include <stdio.h>

#if 0
static struct binary_heap_node *bheap_first_depth(struct binary_heap *bh,
		struct binary_heap_node **parent_stack, int d)
{
	struct binary_heap_node *bhn = bh->top;

	int cd = 0; /* current depth */
	/* decend to 'd' */
	for (;;) {
		parent_stack[cd] = bhn;

		if (!bhn)
			return NULL;
		if (cd == d)
			return bhn;

		bhn = bhn->d[0];
		cd++;
	}
}

static struct binary_heap_node *bheap_next_depth(struct binary_heap_node *bhn,
		struct binary_heap_node **parent_stack, int d)
{
	/* @bhn is a node at depth @d, return the next node at that depth or NULL */
	if (!bhn)
		return NULL;

	assert(0);

	/* TODO: Rise while we are the right node, decend to the right (then
	 * left) as soon as we are the left */
	parent_stack[d - 1];
	for (;;)
		;
}

/* bheap_for_each_at_depth - iterate over the binary heap at a particular
 * depth. This was written so the heap could be visulaized.
 *
 * struct binary_heap *       @bh : the binary heap which is the subject of
 *                                  iteration.
 * struct binary_heap_node ** @parent_stack : size = @depth, used as
 *                                            temporary storage during the
 *                                            iteration.
 * int			      @depth : the depth in the tree to iterate over.
 * struct binary_heap_node *  @bhn : the cursor for the iteration
 */
#define bheap_for_each_at_depth(bh, parent_stack, depth, bhn)	\
	for (bhn = bheap_first_depth(bh, parent_stack, depth);	\
	     bhn;						\
	     bhn = bheap_next_depth(bhn, parent_stack, depth);	\

#endif
