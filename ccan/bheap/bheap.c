/* Licenced under LGPLv3+ - see LICENSE file for details */

/*
 * Heap rules:
 * - "shape property": all levels of the tree except the deepest
 *   are completely filled
 * - "heap property": each node is greater than or equal to each
 *   of its children, according to a comparison predicate defined
 *   for the heap.
 */

/*
 * Heap Implimentation styles:
 * - use an array
 *   - resize it as needed
 *   - alloc in "blocks"
 * - use embedded pointers (intrusive)
 *   - 3 (2 leaves and parent)
 *   - 2 (2 leaves) plus track tree depth and maintain parent stack
 *     for certain operations.
 *
 * - plain array makes sense in VM systems
 * - array via block allocs makes sense in non-VM
 * - embedded pointers allow us to avoid dynamic allocation.
 *   - 2p+stack style allows more compact representation.
 */

#include <ccan/bheap/bheap.h>
#include <ccan/bheap/bheap_poison.h>

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct bheap_node bhn;

/* - every node is always considered to have a parent. For the 'top' node, we
 *   use the bheap itself as the parent.
 * - a "tree's depth" is 0 if there are 0 nodes, 1 with 1 node, 2 with 2-3 nodes, and so on.
 * - a "node's depth" is 0 if it is the top node (at tree depth 0), et c.
 * - parent's (via the parent stack) are indexed by "node depth"
 */

/*
 * Note: the "sliding" pattern often used in array based implimentations of
 * push_down() and bubble_up() could be adopted to this intrusive design, but
 * might not be that useful
 *
 * We need the storage equivalent to the node which is being pushed down to track a slot.
 * The only 1 assignment (which, to be fair, could be repeated o(lg(N)) times)
 * is removed by switching to a slot methodology:
 *
 * - in push_down(), the promoted node has it's leaf set to the cursor node unneededly
 * - in bubble_up(), the node which is bubbling up is unneededly hooked as a leaf of it's new parent.
 */

/*
 * Note: On NULLs terminating the heap
 * In push_down(), we currently rely on the leaves being NULL to indicate the
 * tree has ended. All the same information could be obtained via careful
 * tracking of d_idx and friends (as we do when finding the last node in the
 * tree).
 *
 * If the extra tracking is cheaper than the few extra memory loads of the NULL
 * checking, we'll want to adjust the impl.
 */

/* N=1  + D=1   -- parent_stack = +
 *
 * N=2  + D=2   -- parent_stack = +
 *     /			  |
 *    +				  +
 *
 * N=3  +  D=2   log2(N + 1) = 2  +
 *     / \       log2(N) = 1.098  |
 *    +   +			  +
 *
 * N=4  + D=3 log2(N+1) = 2.321   +(0, points into the struct bheap_)
 *     / \    log2(N) = 1.386	  |
 *    +   +			  +(1)
 *   /				  |
 *  +				  +(2)
 *
 * N=5 + D=3  log2(N+1) = 2.584  +
 *    / \     log2(N)   = 1.606	 |
 *   +   +			 +
 *  / \				 |
 * +   +			 +
 *
 * N=7 + D=3  log2(N + 1) = 3
 *    / \     log2(N) = 1.945
 *   +   +
 *  / \  |\
 * +   + + +
 *
 * N=6, D=3
 * N=7, D=3
 * N=8, D=4
 * N=9-15 D=4
 *
 * Level @l of a tree has nodes L(l) = 2**(l-1)
 * A tree with @l levels has nodes T(l) = 2**l
 * A tree with @n nodes has depth d(n) =  ceil(log2(n+1))
 *
 * It would be desirable to use direct floor: df(n) = floor(log2(n)) + 1
 *
 * Thankfully, df(n) == d(n) for all n > 0 (ie: 0 differs)
 *
 * If the lowest level is full, the value is exact (without ceil).
 *
 * N=7 + D=3 (log2(N + 1) = 3)
 *    / \
 *   +   +
 *  / \  |\
 * +   + + +
 * 0   1 2 3
 *
 * N=15  + D=4 (log2(N + 1) = 3)
 *      / \
 *     +   +
 *    /|   |\
 *   + +   + +
 *  /| |\ /| |\
 * + + ++ ++ + +
 * 0 1 23 45 6 7
 *
 */

/*
 * swap_node_with_parent - Swap a and b, where a is higher in the tree and is
 * currently hooked in via a_slot
 *
 * returns the new parent_slot of parent.
 *
 * [ascii art showing the tree transform]
 *
 *       p1             p1
 *       /              /
 *      a              b
 *     / \     --->   / \
 *    b  c1          a  c1
 *   / \            / \
 * c2a c2b	  c2a c2b
 *
 */
static struct bheap_node **swap_node_with_parent(struct bheap_node *a, struct bheap_node *b,
		struct bheap_node **a_slot)
{
	/* direction of b from a, soon to be direction of a from b */
	bool d_of_b = a->d[0] != b;
	struct bheap_node b_links = *b;
	assert(a->d[d_of_b] == b);
	assert(*a_slot == a);
	*a_slot = b;
	b->d[ d_of_b] = a;
	b->d[!d_of_b] = a->d[!d_of_b];
	*a = b_links;
	return &b->d[d_of_b];
}

#ifdef CCAN_BHEAP_DEBUG
static void poison_parent(struct bheap_node **slot, int depth_mark)
{
	*slot = (void *)(uintptr_t)(BH_POISON_PARENT + depth_mark);
}

static void poison_parent_stack(struct bheap_node **parent_stack, int depth)
{
	/* poison the parent_stack */
	int i;
	for (i = 0; i < depth; i++)
		poison_parent(parent_stack + i, i);
}
#else
static inline void poison_parent(struct bheap_node **slot, int dm)
{
	(void)slot;
	(void)dm;
}

static inline void poison_parent_stack(struct bheap_node **ps, int depth)
{
	(void)ps;
	(void)depth;
}
#endif

/*
 * swap_node_up - Given the node 'c' at depth 'depth', swap it up a level
 *
 *        E
 *       /
 *      A
 *     / \
 *    B   F
 *   / \
 *  C   D
 *
 *  to
 *
 *        E
 *       /
 *      B
 *     / \
 *    A   F
 *   / \
 *  C   D
 *
 * tmp = A
 * A = B
 * B = tmp
 * E[0] = B (instead of A)
 * B[0] = A (instead of B)
 */
static void swap_node_up(struct bheap_node *c, struct bheap_node **parent_stack, int depth)
{
	struct bheap_node *parent = parent_stack[depth];
	struct bheap_node *parent_2 = parent_stack[depth - 1];
	swap_node_with_parent(parent, c, &parent_2->d[parent_2->d[0] != parent]);
	poison_parent(parent_stack + depth, depth);
}

static int bheap_ord(struct _total_order order, size_t offset,
		struct bheap_node *a, struct bheap_node *b)
{
	return total_order_cmp(order, a - offset, b - offset);
}

/* bubble_up - move 'c' up the heap whose parent_stack is given
 * @ord: the ordering function for this heap
 * @c  : the node to "bubble up" the heap based on ord()
 * @parent_stack: a populated parent stack of at least @depth entries
 * @depth: the depth of @c in @parent_stack (such that `parent_stack[depth]` is @c's parent)
 *
 * NOTE: this does not update the parent_stack with the changes made
 */
static void bubble_up(struct _total_order order, size_t offset, struct bheap_node *c,
		struct bheap_node **parent_stack, int depth)
{
	int d;
	for (d = depth - 1; d; d--) {
		/* c >= it's parent */
		if (bheap_ord(order, offset, c, parent_stack[d]) >= 0)
			break;
		swap_node_up(c, parent_stack, d);
	}
}

void bheap_push_(struct bheap_ *b, struct bheap_node *new_node, size_t offset)
{
	/* the depth where the new node (+1) will be inserted */
	int future_tree_depth = bheap_depth(b->sz + 1);

	/* These 3 items form the "context" for the current stack traversal */
	bhn *parent_stack[future_tree_depth];
	poison_parent_stack(parent_stack, future_tree_depth);
	bhn **c = &b->top;
	int d = 0;

	/* Pretend that a pointer to the root's [which has type (struct
	 * bheap)] member named 'top' [which has type (struct
	 * bheap_node *)] is actually a (struct bheap_node *)
	 * itself.
	 *
	 * This relys on code checking ->d[0] first, and not checking ->d[1]
	 * unless that fails (For parent_stack[0], ->d[1] actually looks at the
	 * fields in struct bheap_ that follow ->top. */
	parent_stack[0] = &((union bheap_union *)&b->top)->node;

	/* the number of nodes in the last row.
	 * Also the number of nodes in the tree exculuding the last row.
	 */
	int d_sz  = 1 << (future_tree_depth - 1);
	/* position of the empty spot in the lowest row */
	/* This encodes all the info needed to travel to the node */
	int d_idx = b->sz - d_sz + 1;

	/* Decend to the insert location */
	for (d = 0; d < (future_tree_depth - 1); d++) {
		int h_sz = d_sz / 2;
		parent_stack[d + 1] = *c;
		if (d_idx < h_sz) {
			/* insert point is in the left subtree */
			c = &(*c)->d[0];
		} else {
			/* insert point is in the right subtree */
			c = &(*c)->d[1];
			/* fixup d_idx for this subtree */
			d_idx -= h_sz;
		}
		/* fixup d_sz for new subtree */
		d_sz = h_sz;
	}

	/* Insert new_node */
	new_node->d[0] = new_node->d[1] = NULL;
	*c = new_node;
	b->sz++;

	/* bubble up new_node as needed */
	bubble_up(b->order, offset, new_node, parent_stack, future_tree_depth);
}

/*
 * take last elem and make first, then push down
 *     8
 *    / \
 *   2   3
 *  /|	 |
 * 5 7   4
 *     2
 *    / \
 *   8   3
 *  /|	 |
 * 5 7   4
 *     2
 *    / \
 *   5   3
 *  /|	 |
 * 8 7   4
 *
 * O(lg(N + 1)) steps to move last node to first, and O(lg(N+1) steps to fixup
 * ordering.
 *
 * --------------
 * 2	depth = 2
 * |\
 * 5 3
 *
 * d_sz = 1 << (depth - 1) = 1 << 1 = 2
 * d_idx = b->sz - d_sz = 3 - 2 = 1
 *
 *	c = &b->top = &"2"
 *	d = 1
 *
 *	c = &(*&"2")->d[1] = &3
 *	d = 2
 *
 * done
 *
 */

static void push_down(struct bheap_ *bh, size_t offset)
{
	bhn **parent_slot = &bh->top;

	/* we follow the same node all the way down */
	bhn *c = *parent_slot;

	/* decend back through tree, fixing ordering */
	for (;;) {
		assert(*parent_slot == c);
		if (!c->d[0]) {
			assert(!c->d[1]);
			/* case 1: both children are null */
			/* we are at the bottom, done */
			return;
		} else if (!c->d[1]) {
			/* case 2: right child is null */
			/* check if ord(parent, child) > 0 "parent > child" */
			/* if so, swap and continue to left */

			if (bheap_ord(bh->order, offset, c, c->d[0]) > 0)
				parent_slot = swap_node_with_parent(c,
						c->d[0], parent_slot);
			else
				return;
		} else {
			/* case 3: neither child is null */
			/* establish the ordering amoung the 3 nodes */

			bool p_gt_l = bheap_ord(bh->order, offset, c, c->d[0]) > 0;
			bool p_gt_r = bheap_ord(bh->order, offset, c, c->d[1]) > 0;
			/* promote the smallest one and continue down that leaf. */
			/* if no promotion occurs, we are done */
			if (!p_gt_l) {
				if (!p_gt_r)
					return;
				else
					parent_slot = swap_node_with_parent(c,
							c->d[1], parent_slot);
			} else {
				if (!p_gt_r)
					parent_slot = swap_node_with_parent(c,
							c->d[0], parent_slot);
				else {
					bool l_gt_r = bheap_ord(bh->order, offset, c->d[0], c->d[1]) > 0;
					parent_slot = swap_node_with_parent(c,
							c->d[l_gt_r], parent_slot);
				}
			}
		}
	}
}

static inline struct bheap_node **bheap_get_last_node_slot(struct bheap_ *bh)
{
	if (!bh->sz)
		return NULL;

	/* the depth of the tree prior to removing a node */
	int depth = bheap_depth(bh->sz);

	/* the number of nodes in the last row.
	 * Also the number of nodes in the tree exculuding the last row.
	 */
	int d_sz  = 1 << (depth - 1);
	/* position of the last node in the last row */
	int d_idx = bh->sz - d_sz;

	/* These form the "context" for the current stack traversal */
	bhn **c = &bh->top;
	int d;
	/* Decend to the removal location */
	for (d = 0; d < (depth - 1); d++) {
		int h_sz = d_sz / 2;
		if (d_idx < h_sz) {
			/* insert point is in the left subtree */
			c = &(*c)->d[0];
		} else {
			/* insert point is in the right subtree */
			c = &(*c)->d[1];
			/* fixup d_idx for this subtree */
			d_idx -= h_sz;
		}
		/* fixup d_sz for new subtree */
		d_sz = h_sz;
	}

	return c;
}

struct bheap_node *bheap_pop_(struct bheap_ *bh, size_t offset)
{
	bhn **c = bheap_get_last_node_slot(bh);
	if (!c)
		return NULL;

	/* unhook the last node */
	bhn *last = *c;
	*c = NULL;
	bh->sz --;

	/* no need to continue if we're out of nodes */
	/* XXX: should we special case this sooner? */
	if (c == &bh->top)
		return last;

	/* insert old last node at the top */
	bhn *top = bh->top;
	bh->top = last;
	*last = *top;

	push_down(bh, offset);

	return top;
}
