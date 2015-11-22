#include <ccan/bheap/bheap.h>
#include <ccan/time/time.h>
#include <ccan/array_size/array_size.h>
#include <ccan/heap/heap.h>

#include <ccan/bheap/bheap.c>
#include <ccan/ilog/ilog.c>
#include <ccan/time/time.c>
#include <ccan/heap/heap.c>

#include <ccan/bheap/test/helper_X.h>

#include <time.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

bool X_less(const void *a_, const void *b_)
{
	const int *a = a_, *b = b_;
	return *a < *b;
}

struct t2 {
	struct timerel t[2];
};

static struct t2 bheap_pp_N(int len)
{
	struct bheap bh = BHEAP_INIT(X_ord);
	struct X *x = malloc(sizeof(*x) * len);
	int i;
	for (i = 0; i < len; i++) {
		x[i].v = rand() % (len * 4);
	}

	struct timerel t1, t2, t3;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1.ts);

	for (i = 0; i < len; i++)
		bheap_push(&bh, &x[i].bhn);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2.ts);

	for (i = 0; i < len; i++)
		bheap_pop(&bh);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t3.ts);

	free(x);
	return (struct t2){{ time_sub(t2, t1), time_sub(t3, t2) }};
}

static struct t2 aheap_pp_N(int len)
{
	struct heap bh = HEAP_INIT(X_less);
	int *x = malloc(sizeof(*x) * len);
	int i;
	for (i = 0; i < len; i++) {
		x[i] = rand() % (len * 4);
	}

	struct timerel t1, t2, t3;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t1.ts);

	for (i = 0; i < len; i++)
		heap_push(&bh, &x[i]);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t2.ts);

	for (i = 0; i < len; i++)
		heap_pop(&bh);

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t3.ts);
	free(x);
	free(bh.data);
	return (struct t2){{ time_sub(t2, t1), time_sub(t3, t2) }};
}

static void p_ns(const char *n, struct t2 t)
{
	printf("%17s : %8"PRIu64" %8"PRIu64" %8"PRIu64"\n",
		n,
		time_to_nsec(t.t[0]),
		time_to_nsec(t.t[1]),
		time_to_nsec(timerel_add(t.t[0], t.t[1])));
}

static void pp_N(int len)
{

	printf("pop-n-push-n %5d: ns-per-push ns-per-pop ns-combined\n", len);

	int r = 10000000 / len;
	struct t2 a[2] = {};
	int i;
	for (i = 0; i < r; i++) {
		struct t2 r = bheap_pp_N(len);
		a[0].t[0] = timerel_add(a[0].t[0], r.t[0]);
		a[0].t[1] = timerel_add(a[0].t[1], r.t[1]);
	}

	a[0].t[0] = time_divide(a[0].t[0], len * r);
	a[0].t[1] = time_divide(a[0].t[1], len * r);

	p_ns("bheap", a[0]);

	for (i = 0; i < r; i++) {
		struct t2 r = aheap_pp_N(len);
		a[1].t[0] = timerel_add(a[1].t[0], r.t[0]);
		a[1].t[1] = timerel_add(a[1].t[1], r.t[1]);
	}

	a[1].t[0] = time_divide(a[1].t[0], len * r);
	a[1].t[1] = time_divide(a[1].t[1], len * r);

	p_ns("aheap", a[1]);
}

int main(void)
{
	pp_N(10);
	pp_N(50);
	pp_N(100);
	pp_N(3000);
	pp_N(30000);

	return 0;
}
