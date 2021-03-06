/* Licensed under LGPLv2+ - see LICENSE file for details */
#include "config.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include <ccan/aga/aga.h>
#include "private.h"

/*
 * Depth first search
 */

static bool dfs_push(struct aga_graph *g, struct lstack *stack,
		     struct aga_node *n)
{
	if (!aga_update_node(g, n))
		return false;

	lstack_push(stack, n, u.dfs.parent);
	n->u.dfs.edge = aga_first_edge(g, n);
	return true;
}

static void dfs_pop(struct lstack *stack)
{
	lstack_pop(stack, struct aga_node, u.dfs.parent);
}

static struct aga_node *dfs_top(struct lstack *stack)
{
	return lstack_top(stack, struct aga_node, u.dfs.parent);
}

int aga_dfs_start(struct aga_graph *g)
{
	int rc;

	rc = aga_start(g);
	if (rc < 0)
		return rc;

	return 0;
}

struct aga_node *aga_dfs_explore(struct aga_graph *g, struct aga_node *n)
{
	LSTACK(stack);

	if (!aga_check_state(g))
		return NULL;

	if (!n)
		return NULL;

	if (dfs_push(g, &stack, n))
		return n;

	lstack_init_from_top(&stack, n, u.dfs.parent);

	while ((n = dfs_top(&stack))) {
		const void *e = n->u.dfs.edge;
		int err;
		struct aga_edge_info ei;

		if (!e) {
			/* out of edges, back up */
			dfs_pop(&stack);
			continue;
		}

		n->u.dfs.edge = aga_next_edge(g, n, e);

		err = aga_edge_info(g, n, e, &ei);
		if (err < 0) {
			aga_fail(g, err);
			return NULL;
		}
		if (!ei.to) {
			/* missing edge */
			continue;
		}

		if (!dfs_push(g, &stack, ei.to)) {
			/* already visited node */
			continue;
		}

		return ei.to;
	}
	
	return NULL;
}
