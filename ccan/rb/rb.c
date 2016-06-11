/* GNU LGPL version 2 (or later) - see LICENSE file for details */

/*
 * CLRS
 *
 * CLR
 *
 * http://cs.wellesley.edu/~cs231/fall01/red-black.pdf
 */
#include <ccan/rb/rb.h>

#include <assert.h>

/*
 * rb_parent variant that assumes we are red
 */
static struct rb_node *
rb_red_parent(const struct rb_node *n)
{
	assert(rb_is_red(n));
	return (struct rb_node *)n->parent_and_color;
}

static void
rb_color_red(struct rb_node *rbn)
{
	rbn->parent_and_color &= ~(uintptr_t)1;
}

static void
rb_color_black(struct rb_node *rbn)
{
	rbn->parent_and_color |= 1;
}

static void
rb_color_copy(struct rb_node *dest, const struct rb_node *src)
{
	if (rb_is_black(src))
		rb_color_black(dest);
	else
		rb_color_red(dest);
}

static
struct rb_node *
rb_minimum(const struct rb_node *n)
{
	while (n->c[0])
		n = n->c[0];
	return (struct rb_node *)n;
}

static
struct rb_node *
rb_maximum(const struct rb_node *n)
{
	while (n->c[1])
		n = n->c[1];
	return (struct rb_node *)n;
}


/*
 * x: the parent node
 * right: if true, rotate right, if false rotate left
 *
 * y is the child to the right of x
 *
 *     y         x
 *    / \       / \
 *   x   c     a   y
 *  / \           / \
 * a   b         b   c
 *
 *     <- left  rotate <-
 *     -> right rotate ->
 */
static void
rb_rotate(struct rb_tree *rbt, struct rb_node *x, bool right)
{
	/*
	 * names correspond to a left rotate,
	 * right=false=left
	 * !right=true=right
	 *
	 * q
	 *  \
	 *   x
	 *  / \
	 * a   y
	 *    / \
	 *   b   c
	 */
	/* y = x.right */
	struct rb_node *y = x->c[!right];

	/* x.right = y.left */
	struct rb_node *xr = x->c[!right] = y->c[right];

	/* if (y.left != T.nil) */
	if (xr)
		/* y.left.p = x */
		rb_parent_set(xr, x);

	/* y.p = x.p */
	struct rb_node *xp = rb_parent(x);
	rb_parent_set(y, xp);

	/* if x.p == T.nil */
	if (!xp)
		/* T.root = y */
		rbt->top = y;
	else
		/* elseif x == x.p.left
		 *   x.p.left = y
		 * else
		 *   x.p.right = y
		 */
		xp->c[x != xp->c[0]] = y;


	/* y.left = x */
	y->c[right] = x;


	/* x.p = y */
	rb_parent_set(x, y);
}

static void
rb_insert_fixup(struct rb_tree *rbt, struct rb_node *z)
{
	/* 'z' (the newly inserted node) is always red */
	struct rb_node *parent = rb_red_parent(z);
	for (;;) {
		/* 'parent' is always red.
		 * if it is black or we're at the top of the tree, we're done.
		 */
		if (!parent) {
			z->parent_and_color = rb_parent_color(NULL, RB_BLACK);
			return;
		} else if (rb_is_black(parent))
			return;

		/* 'z.p.p' */
		struct rb_node *gparent = rb_red_parent(parent);
		bool uncle_side = parent == gparent->c[0];

		/*
		 * if parent is the left node, y is the right node (and vise versa)
		 * y = uncle of 'z'
		 */
		struct rb_node *y = gparent->c[uncle_side];
		if (rb_is_red(y)) {
			/* case 1, z's uncle y is red
			 *     C[b]                 C[b]
			 *    /  \                 /  \
			 *   A[r] D[r]y    or     B[r] D[r]y
			 *  /  \                 /  \   / \
			 *      B[r]z         A[r]z
			 *
			 * to
			 *     C[r]z              C[r]z
			 *    /  \                /   \
			 *   A[b] D[b]          B[b]  D[b]
			 *  /  \                /  \   / \
			 *      B[r]         A[r]z
			 */
			rb_color_black(parent);
			rb_color_black(y);
			rb_color_red(gparent);
			z = gparent;
			parent = rb_parent(z);
		} else {
			/* if z is a child of parent (opposite of */
			if (z == parent->c[uncle_side]) {
				/* case 2: z's uncle y is black & z is a right
				 * child
				 *
				 *      C[b]
				 *     /  \
				 *    A[r] y
				 *     \
				 *      B[r]z
				 *
				 *      to
				 */
				z = parent;
				rb_rotate(rbt, z, !uncle_side);
				parent = gparent;
				y = z->c[uncle_side];
			}
			/* case 3: z's uncle y is black & z is a left child
			 *
			 *
			 *
			 */
			rb_color_black(parent);
			rb_color_red(gparent);
			rb_rotate(rbt, gparent, uncle_side);
		}
	}
}

void
rb_insert(struct rb_tree *rbt,
		struct rb_node *parent, struct rb_node **slot, struct rb_node *new)
{
	new->c[0] = new->c[1] = NULL;
	new->parent_and_color = rb_parent_color(parent, RB_RED);
	*slot = new;

	rb_insert_fixup(rbt, new);
}

void
rb_insert_ord(struct rb_tree *rbt, struct rb_node *z, rb_ord_t ord)
{
	struct rb_node **slot = &rbt->top;
	struct rb_node  *x    = *slot,
			*y    = NULL;
	while (x) {
		y = x;
		slot = &x->c[ord(rbt, z, x) >= 0];
		x = *slot;
	}

	rb_insert(rbt, y, slot, z);
}

static
void
rb_fixup_delete(struct rb_tree *rbt, struct rb_node *parent)
{
	struct rb_node *tmp,
		       *x = NULL;
	assert(parent);
	/* parent is never NULL except when we reach the root */

	while (rb_is_black(x)) {
		/* direction of the sibling ('w') to 'x' aka 'node' */
		bool right = x == parent->c[0];

		/* in the algorithm '.right' becomes '->c[right]',
		 * '.left' becomes '->c[!right]'.
		 */

		/* w is x's sibling */
		struct rb_node *w = parent->c[right];
		if (rb_is_red(w)) {
			/* case 1 */
			/*
			 *   p
			 *  / \
			 * x   w
			 *    / \
			 *   b   g
			 *
			 *   to
			 *
			 *     w
			 *    / \
			 *   p   g
			 *  / \
			 * x   b
			 */
			tmp = w->c[!right];
			rb_color_black(w);
			rb_color_red(parent);
			rb_rotate(rbt, parent, !right);
			/*
			 * often, one will see:
			 *   w = rb_parent(x)->c[right]
			 * instead. The formulation here is eqivalent and does
			 * not require that x is non-null.
			 */
			w = tmp;
		}

		if (rb_is_black(w->c[0]) && rb_is_black(w->c[1])) {
			/* case 2 */
			rb_color_red(w);
			x = parent;
			parent = rb_parent(parent);
			if (!parent)
				return;
		} else {
			if (rb_is_black(w->c[right])) {
				/* case 3 */
				tmp = w->c[!right];
				rb_color_black(w->c[!right]);
				rb_color_red(w);
				rb_rotate(rbt, w, right);
				/*
				 * A right rotation promotes the pivot's left
				 * child.
				 *
				 * w = rb_parent(x)->c[right];
				 */
				w = tmp;
			}

			/* case 4 */
			rb_color_copy(w, parent);
			rb_color_black(parent);
			rb_color_black(w->c[right]);
			rb_rotate(rbt, parent, !right);
			/* x = rbt->top; */
			rb_color_black(rbt->top);
			return;
		}
	}
}

static struct rb_node *
tree_remove(struct rb_tree *rbt, struct rb_node *node)
{
	struct rb_node *rebalance;
	struct rb_node *np = rb_parent(node);
	struct rb_node *repl;
	bool remove_black = rb_is_black(node);
	
	if (!node->c[0] && !node->c[1]) {
		/* if z has no children, we modify it's parent p[z] to replace z with
		 * NIL as it's child */
		if (np)
			np->c[np->c[0] != node] = NULL;
		else
			rbt->top = NULL;
		rebalance = np;
		rb_node_poison(node, 0x10);
	} else if (node->c[0] && !node->c[1]) {
		repl = node->c[0];
		if (np)
			np->c[np->c[0] != node] = repl;
			/* XXX: consider if copying color here is right */
		else
			rbt->top = repl;
		repl->parent_and_color = node->parent_and_color;
		rebalance = np;
		rb_node_poison(node, 0x20);
	} else if (node->c[1] && !node->c[0]) {
		repl = node->c[1];
		if (np)
			np->c[np->c[0] != node] = repl;
		else
			rbt->top = repl;
		/* XXX: consider if copying color here is right */
		repl->parent_and_color = node->parent_and_color;
		rebalance = np;
		rb_node_poison(node, 0x30);
	} else {
		/* splice out z's successor y, which has no left child and
		 * replace z's key & satellite data with y's key and satellite
		 * data */
		repl = rb_minimum(node->c[1]);
		remove_black = rb_is_black(repl);
		struct rb_node *rc = repl->c[1];
		rebalance = rb_parent(repl);
		bool repl_dir = rebalance->c[0] != repl;
		bool node_dir = np->c[0] != node;

		/*
		 * keep in mind that `node` may equal `rebalance` (repl's parent).
		 * remove `repl` before inserting it into `node`'s posision to
		 * avoid problems when repl is node's child
		 */
		if (rc)
			rc->parent_and_color = repl->parent_and_color;
		rebalance->c[repl_dir] = rc;

		if (rebalance == node)
			rebalance = repl;

		*repl = *node;
		if (np)
			np->c[node_dir] = repl;
		else
			rbt->top = repl;
		if (repl->c[0])
			rb_parent_set(repl->c[0], repl);
		if (repl->c[1])
			rb_parent_set(repl->c[1], repl);


		rb_node_poison(node, 0x40);
	}

	return remove_black ? rebalance : NULL;
}

void
rb_remove(struct rb_tree *rbt, struct rb_node *node)
{
	struct rb_node *rebalance = tree_remove(rbt, node);
	if (rebalance)
		rb_fixup_delete(rbt, rebalance);
}

#if 0
void
rb_remove(struct rb_tree *rbt, struct rb_node *node)
{
	struct rb_node *y = node,
		       *x,
		       *rebalance;

	bool orig_black = rb_is_black(y);
	if (!node->c[0]) {
		x = node->c[1];
		rebalance = rb_parent(node);
		rb_replace(rbt, node, x);
	} else if (!node->c[1]) {
		x = node->c[0];
		rebalance = rb_parent(node);
		rb_replace(rbt, node, x);
	} else {
		y = rb_minimum(node->c[1]);
		orig_black = rb_is_black(y);
		x = y->c[1];
		rebalance = y;
		if (rb_parent(y) == node)
			rb_parent_set(y, node);
		else {
			rb_replace(rbt, y, y->c[1]);
			y->c[1] = node->c[1];
			rb_parent_set(y->c[1], y);
		}

		rb_replace(rbt, node, y);
		y->c[0] = node->c[0];
		rb_parent_set(y->c[0], y);
		rb_color_copy(y, node);
	}

	if (orig_black)
		rb_fixup_delete(rbt, rebalance);
}
#endif

/* iteration */

struct rb_node *
rb_first(const struct rb_tree *rbt)
{
	if (rbt->top)
		return rb_minimum(rbt->top);
	else
		return NULL;
}

struct rb_node *
rb_last(const struct rb_tree *rbt)
{
	return rb_maximum(rbt->top);
}

struct rb_node *
rb_next(const struct rb_node *x)
{
	struct rb_node *n = x->c[1];
	if (n)
		return rb_minimum(n);

	struct rb_node *y = rb_parent(x);
	while (y && x == y->c[1]) {
		x = y;
		y = rb_parent(y);
	}
	return y;
}

static
struct rb_node *
rb_lowest_bias_left(const struct rb_node *c)
{
	for(;;) {
		if (c->c[0])
			c = c->c[0];
		else if (c->c[1])
			c = c->c[1];
		else
			return (struct rb_node *)c;
	}
}

struct rb_node *
rb_first_postorder(const struct rb_tree *rbt)
{
	if (rbt->top)
		return rb_lowest_bias_left(rbt->top);
	else
		return NULL;
}

struct rb_node *
rb_next_postorder(const struct rb_node *c)
{
	/* we're at this node, so we've already looked at all our children */
	struct rb_node *n = rb_parent(c);

	/* if we were at the left node, get the lowest node on the right */
	if (n && n->c[0] == c)
		return rb_lowest_bias_left(n->c[1]);

	/* otherwise, we were the right node, we we should now look at the
	 * parent */
	return n;
}
