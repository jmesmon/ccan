#include "../rb.h"

#include <ccan/container_of/container_of.h>
#include <ccan/compiler/compiler.h>
#include <ccan/order/order.h>
#include <ccan/str/str.h>

#include <string.h>
#include <stdio.h>

#define COLL_TYPE struct rb_tree
#define ELEM_TYPE struct rb_node

#define COLL_DECL(name_) \
	struct rb_tree name_ = RB_TREE_INIT

#define elem_to_X(b) container_of(b, struct X, e)

struct X {
	ELEM_TYPE e;
	const char *v;
	int extra;
};

static UNNEEDED
int X_cmp_elem(const COLL_TYPE *bh, const ELEM_TYPE *a, const ELEM_TYPE *b)
{
	(void)bh;
	return strcmp(elem_to_X(a)->v, elem_to_X(b)->v);
}

static UNNEEDED
int X_cmp(const struct X *a, const struct X *b)
{
	return strcmp(a->v, b->v);
}

static UNNEEDED bool
rb_is_leaf(struct rb_node *x)
{
	return (!x->c[0]) && (!x->c[1]);
}

#define eq(a, b) ((a) == (b))
#define assert_op_fmt(fmt, op, opstr, a, b) do { \
	if (!(op((a),(b)))) { \
		fprintf(stderr, "%s:%d: %s: Assertion `"#a" "opstr" "#b"`, aka `%"fmt" "opstr" %"fmt"` failed.\n", \
				__FILE__, __LINE__, __func__, a, b); \
		abort(); \
	} \
} while(0)
#define assert_eq_fmt(fmt, a, b) assert_op_fmt(fmt, eq, "==", a, b)

#define assert_eq_zu(a, b) assert_eq_fmt("zu", a, b)
#define assert_eq_p(a, b) assert_eq_fmt("p", a, b)
#define assert_ne_s(a, b) assert_op_fmt("s", strcmp, "!=", a, b)

/* decends a tree with parent pointers and confirms the parent pointers are
 * correct */
static void UNNEEDED
rb_assert_parents(struct rb_node *n, struct rb_node *parent)
{
	if (!n)
		return;
	assert_eq_p(rb_parent(n), parent);
	rb_assert_parents(n->c[0], n);
	rb_assert_parents(n->c[1], n);
}

static void UNNEEDED
rb_assert_tree(struct rb_tree *rbt)
{
	rb_assert_parents(rbt->top, NULL);
}

/* tap helpers */
#define ok_eq(a, b) ok_eq_(__func__, __FILE__, __LINE__, false, a, #a, b, #b)
#define bad_eq(a, b) ok_eq_(__func__, __FILE__, __LINE__, true, a, #a, b, #b)
static UNNEEDED
void ok_eq_(const char *func, const char *file, unsigned line, bool inv, const char *a, const char *as, const char *b, const char *bs)
{
	_gen_result(inv ? !!strcmp(a, b) : !strcmp(a, b),
		func, file, line,
		"%s <%s> %s <%s> %s",
		as, a, inv ? "!=" : "==", b, bs);
}

static UNNEEDED
void
X_print_tree(const char *name, struct rb_tree *tr, FILE *f)
{
	fprintf(f, "digraph \"%s\"\n{\n", name);
	struct rb_node *i;
	for (i = rb_first(tr); i; i = rb_next(i)) {
		struct X *it = elem_to_X(i);
		fprintf(f, "%s [color=\"%s\"];\n",
			it->v,
			rb_is_red(i) ? "red" : "black");

		unsigned j;
		for (j = 0; j < 2; j++) {
			if (i->c[j]) {
				struct X *c = elem_to_X(i->c[j]);
				fprintf(f, "%s -> %s;\n",
						it->v, c->v);
			}
		}
	}
	fputc('}', f);
}

static UNNEEDED
void
print_tree_h(struct rb_node *n, struct rb_node *parent, FILE *stream)
{
	if (!n)
		return;

	if (parent)
		fprintf(stream,"n%p [color=\"%s\"];"
			"\nn%p -> n%p;\n"
			, n
			, rb_is_red(n) ?"red":"black"
			, parent
			, n);
	else
		fprintf(stream,"n%p [color=\"%s\"];\n"
			,n
			,rb_is_red(n) ? "red" : "black");
	print_tree_h(n->c[0],n,stream);
	print_tree_h(n->c[1],n,stream);
}

static UNNEEDED
void
print_tree(struct rb_tree *tr, FILE *stream)
{
	fputs("digraph \"tree\"\n{\n",stream);
	print_tree_h(tr->top, 0, stream);
	fputs("}\n",stream);
}

/*
 * returns the black depth of the sub-tree
 */
static UNNEEDED
size_t
rb_assert_(struct rb_node *pos)
{
	/* NULL is black */
	if (!pos)
		return 1;

	/* red nodes must have black children */
	if (rb_is_red(pos)) {
		assert(!rb_is_red(pos->c[0]));
		assert(!rb_is_red(pos->c[1]));
	}

	if (pos->c[0])
		assert_eq_p(rb_parent(pos->c[0]), pos);
	if (pos->c[1])
		assert_eq_p(rb_parent(pos->c[1]), pos);

	size_t l = rb_assert_(pos->c[0]),
	       r = rb_assert_(pos->c[1]);

	/* black depth must be equal */
	assert_eq_zu(l, r);

	return rb_is_black(pos) + l;
}

static UNNEEDED
void
rb_assert(struct rb_tree *rbt)
{
	assert(!rb_is_red(rbt->top));
	if (rbt->top)
		assert_eq_p(rb_parent(rbt->top), NULL);
	rb_assert_(rbt->top);
}
