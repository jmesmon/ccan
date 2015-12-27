/* GNU LGPL version 2 (or later) - see LICENSE file for details */
#ifndef CCAN_RB_H_
#define CCAN_RB_H_

#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include <ccan/alignof/alignof.h>
#include <ccan/build_assert/build_assert.h>

#define RB_RED   false
#define RB_BLACK true
#define RB_LEFT  false
#define RB_RIGHT true

struct rb_tree;
struct rb_node;

typedef int (*rb_ord_t)(const struct rb_tree *, const struct rb_node *left, const struct rb_node *right);

/**
 * struct rb_node - a node in the tree
 *
 * @parent_and_color: parent node pointer with color of the current node
 *                      stored in the low bits. Use the rb_parent() and
 *                      rb_is_read() accessors.
 * @c: child nodes
 *
 *
 * Example:
 *
 * struct foo {
 *	int x;
 *	struct rb_node rb;
 * };
 */
struct rb_node {
	uintptr_t parent_and_color;
	struct rb_node *c[2];
};

/**
 * struct rb_tree - the tree root
 *
 * @top: the first node, NULL if the tree is empty.
 */
struct rb_tree {
	struct rb_node *top;
};

#ifdef CCAN_RB_DEBUG
# define RB_POISON_BASE 0xdead0000
# define RB_POISON_NODE (RB_POISON_BASE | 0x8000)
static inline void
rb_node_poison(struct rb_node *n, uintptr_t extra)
{
	n->c[0] = (void *)(uintptr_t)(RB_POISON_NODE | extra);
	n->c[1] = (void *)(uintptr_t)(RB_POISON_NODE | extra | 1);
	n->parent_and_color = (uintptr_t)(RB_POISON_NODE | extra | 3);
}
#else
static inline void
rb_node_poison(struct rb_node *n, uintptr_t extra)
{
	(void)n;
	(void)extra;
}
#endif

/**
 * RB_TREE_INIT - initializer for a 'struct rb_tree'
 */
#define RB_TREE_INIT { \
	NULL \
}

/**
 * rb_tree_init - initialize a 'struct rb_tree'
 * @rbt: the tree to initialize
 */
static inline void
rb_tree_init(struct rb_tree *rbt)
{
	rbt->top = NULL;
	BUILD_ASSERT(ALIGNOF(struct rb_node) > 1);
}

/**
 * rb_parent - return the node's parent
 * @n: node, must be part of a tree (ie: initialized)
 *
 * Returns the parent node.
 */
static inline struct rb_node *
rb_parent(const struct rb_node *n)
{
	return (struct rb_node *)(n->parent_and_color & ~(uintptr_t)1);
}

/**
 * rb_parent_set - set a node's parent
 * @n: node
 * @p: new parent for node
 */
static inline void
rb_parent_set(struct rb_node *n, struct rb_node *p)
{
	assert(((uintptr_t)p & 1) == 0);
	n->parent_and_color = (n->parent_and_color & 1) | (uintptr_t)p;
}

/**
 * rb_parent_color - generate a parent + color value
 * @p: parent
 * @black: true if black
 */
static inline uintptr_t
rb_parent_color(struct rb_node *p, bool black)
{
	assert(((uintptr_t)p & 1) == 0);
	return (uintptr_t)p | black;
}

/* modification */

/**
 * rb_replace - replace a node that is in a tree with a new one that has the same 'key'/ordering.
 * @root: the root of the tree
 * @victim: the node to replace, must be a tree member
 * @repl: the node to be inserted (replacement)
 *
 *
 * AKA: transplant
 */
static inline void
rb_replace(struct rb_tree *root, struct rb_node *victim, struct rb_node *repl)
{
	*repl = *victim;
	struct rb_node *p = rb_parent(repl);
	if (p)
		p->c[p->c[0] != victim] = repl;
	else
		root->top = repl;
	if (repl->c[0])
		rb_parent_set(repl->c[0], repl);
	if (repl->c[1])
		rb_parent_set(repl->c[1], repl);

	rb_node_poison(victim, 0x1000);
}

/**
 * rb_insert - add a new node to an rb_tree
 * @parent: the node that will be the parent of the node to be inserted
 * @slot: the slot of @parent (one of &@parent->c[0] or &@parent->c[1]) where @new will be inserted
 * @n: the new node
 *
 * Runtime is O(lg(n))
 * At most 2 rotations are performed
 *
 * Reorders tree, iteration continued across this may miss nodes.
 */
void
rb_insert(struct rb_tree *rbt,
		struct rb_node *parent, struct rb_node **slot, struct rb_node *n);

/**
 * rb_insert_ord - add a new node to an rb_tree, given an ordering function
 * @rbt: tree
 * @z: new node to insert
 * @ord: ording function
 *
 */
void
rb_insert_ord(struct rb_tree *rbt, struct rb_node *z, rb_ord_t ord);

/**
 * rb_remove - remove a node from a tree
 * @node: node that belongs to a tree to remove
 *
 * O(1), max of 3 rotations.
 *
 * Reorders tree, iteration continued across this may miss nodes.
 *
 * Removed node contents (@node) is left in an undefined state.
 */
void
rb_remove(struct rb_tree *rbt, struct rb_node *node);

/* iteration */

/**
 * rb_first - first node in in-order traversal
 * @rbt: the tree to return the first node of
 *
 * O(lg(n))
 *
 * Returns the first node.
 */
struct rb_node *
rb_first(const struct rb_tree *rbt);

/**
 * rb_next - next node in in-order traversal
 * @node: the current node
 *
 * O(1)
 *
 * Returns the next node.
 */
struct rb_node *
rb_next(const struct rb_node *node);

/**
 * rb_last - last node in a in-order traversal
 * @rbt: the tree to return the last node of
 *
 * Returns the last node, or NULL if the tree is empty.
 */
struct rb_node *
rb_last(const struct rb_tree *rbt);

/**
 * rb_first_postorder - first node in postorder (children before parents) traversal
 * @rbt: the tree to iterate over
 *
 * Post-order iteration is useful for destroying an entire tree quickly
 * (compared to normal iteration + erase).
 *
 * O(lg(n))
 *
 * Returns the first node.
 */
struct rb_node *
rb_first_postorder(const struct rb_tree *rbt);

/**
 * rb_next_postorder - next node in postorder (children before parents) traversal
 * @node: the current node
 *
 * Returns the next node.
 */
struct rb_node *
rb_next_postorder(const struct rb_node *node);

/* internal, probably unneeded in production code outside of rbtree source */

/**
 * rb_is_red - is this a red node? (ie: not black)
 * @n: node
 *
 * Returns true if the node is red, false if it is black
 */
static inline bool
rb_is_red(const struct rb_node *n)
{
	return n && !(n->parent_and_color & 1);
}

static inline bool
rb_is_black(const struct rb_node *n)
{
	return (!n) || (n->parent_and_color & 1);
}

#endif
