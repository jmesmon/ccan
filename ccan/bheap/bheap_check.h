/* Licenced under LGPLv3+ - see LICENSE file for details */

#ifndef BHEAP_CHECK_H_
#define BHEAP_CHECK_H_

#include <ccan/bheap/bheap.h>
#include <assert.h>

static void _bheap_check_depth(struct binary_heap_node *n, int c, int depth, int d_sz, int d_idx)
{
	if (!n) {
		assert(c == depth || (d_idx == 0 && c == (depth - 1)));
		return;
	}

	int h_sz = d_sz / 2;
	int r_idx = d_idx - h_sz;
	if (r_idx < 0)
		r_idx = 0;
	_bheap_check_depth(n->d[0], c + 1, depth, h_sz, d_idx);
	_bheap_check_depth(n->d[1], c + 1, depth, h_sz, r_idx);
}

static inline void bheap_check_depth(struct binary_heap *b)
{
	/* max depth, min = @depth - 1 */
	int depth = ceil_ilog_zu(b->sz + 1);
	/* num elems in depth - 1 rows, num elems possible in last row */
	int d_sz  = 1 << (depth - 1);
	/* actual number of elements in the last row */
	int d_idx = b->sz - d_sz + 1;

	_bheap_check_depth(b->top, 0, depth, d_sz, d_idx);
}


#endif
