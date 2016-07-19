/* GNU LGPL - see LICENSE file for details */
#ifndef CCAN_VEC_H
#define CCAN_VEC_H

#include <ccan/compiler/compiler.h>
#include <ccan/tcon/tcon.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <errno.h>

struct vec_ {
	void *data;
	/* Bytes occupied by valid data (or returned to the caller with the
	 * intent of them immediately filling the bytes) */
	size_t used;

	/* Total bytes in data that may be accessed */
	size_t capacity;
};

/**
 * VEC - declare a vector with elements of a particular type
 * @etype: the type of the elements of this vector
 *
 * Example:
 *	VEC(int) nums;
 *
 */
#define VEC(etype) TCON_WRAP(struct vec_, etype elem)

/**
 * VEC_INIT - initializer for a VEC
 *
 * Example:
 *	VEC(char *) strings = VEC_INIT;
 */
#define VEC_INIT TCON_WRAP_INIT({NULL, 0, 0})

/**
 * VEC_ELEM - return the type of the element (if supported)
 * @vec: &VEC(...)
 */
#if HAVE_TYPEOF
#define VEC_ELEM(vec_) tcon_type(vec_, elem)
#else
#define VEC_ELEM(vec_) void
#endif

/**
 * vec_init - initialize an already declared vec
 * @vec: &VEC(...): a vector to initialize
 *
 * Example:
 *	VEC(int) nums;
 *	vec_init(&nums);
 */
#define vec_init(vec) vec_init_(tcon_unwrap(vec))
static inline void vec_init_(struct vec_ *v)
{
	*v = (struct vec_){ NULL, 0, 0};
}

static inline int vec_ensure_space_(struct vec_ *v, size_t e_sz)
{
	if (e_sz <= (v->capacity - v->used))
		return 0;

	size_t nc = v->capacity * 2;
	/* TODO: consider if growing by only powers-of-2 would be useful. If
	 * so, round up here */
	if ((nc - v->used) < e_sz)
		nc += e_sz;
	void *nd = realloc(v->data, nc);
	if (!nd)
		return -ENOMEM;
	v->data = nd;
	v->capacity = nc;
	return 1;
}

static inline int vec_ensure_space_n_(struct vec_ *v, size_t elem_ct, size_t elem_sz)
{
	if (SIZE_MAX / elem_ct <= elem_sz)
		return -ENOMEM;
	return vec_ensure_space_(v, elem_ct * elem_sz);
}

/**
 * vec_append_space - return a pointer to a slot in the vec to populate
 * @vec: &VEC(...): the vector
 *
 * Returns NULL on failure.
 * Can only fail due to memory exaustion.
 *
 * Example:
 *	*vec_append_space(&nums) = 10;
 *	int *l = vec_append_space(&nums);
 *	if (l) { *l = 44; }
 */
#define vec_append_space(vec) (tcon_cast_ptr((vec), elem, vec_append_space_(tcon_unwrap(vec), tcon_sizeof((vec), elem))))
WARN_UNUSED_RESULT
static inline void *vec_append_space_(struct vec_ *v, size_t e_sz)
{
	if (vec_ensure_space_(v, e_sz) < 0)
		return NULL;
	void *l = (char *)v->data + v->used;
	v->used += e_sz;
	return l;
}

/**
 * vec_append_space_n - return a pointer to N slots in the vec to populate
 * @vec: &VEC(...): the vector
 * @n: size_t: number of elements to make space for
 *
 * Returns NULL on failure.
 * Can only fail due to memory exaustion.
 *
 * Example:
 *	int *spaces = vec_append_space_n(&nums, 3);
 *	spaces[0] = 2;
 *	spaces[1] = 5;
 *	spaces[2] = 4;
 */
#define vec_append_space_n(vec, n) (tcon_cast_ptr((vec), elem, vec_append_space_n_(tcon_unwrap(vec), (n), tcon_sizeof((vec), elem))))
WARN_UNUSED_RESULT
static inline void *vec_append_space_n_(struct vec_ *v, size_t elem_ct, size_t elem_sz)
{
	if (SIZE_MAX / elem_ct <= elem_sz)
		return NULL;
	return vec_append_space_(v, elem_ct * elem_sz);
}

WARN_UNUSED_RESULT
static inline int vec_warn_unused_result_(int r)
{
	return r;
}

/**
 * vec_append - add an element to the end of the vector
 * @vec_: &VEC(...): an initialized vector
 * @elem_: (elem_type): the element to append
 *
 * Returns a negative int on failure.
 * Can only fail due to memory exaustion.
 *
 * Example:
 *	int r = vec_append(&nums, 12);
 *	if (r < 0)
 *		return r;
 */
#define vec_append(vec_, elem_) vec_warn_unused_result_(__extension__({ \
	int vec_append__r_ = 0; \
	tcon_ptr_type((vec_), elem) vec_append__l_ = vec_append_space(vec_); \
	if (!vec_append__l_) \
		vec_append__r_ = -1; \
	*vec_append__l_ = elem_; \
	vec_append__r_; \
}))

/**
 * vec_concat - append a vec to the end of another vec
 * @base: the vec to extend
 * @extra: the vec to read data from to extend @base
 *
 * Can only fail due to memory exaustion.
 */
#define vec_concat(base_, extra_) vec_concat_(tcon_unwrap(tcon_check_eq(base_, extra_, elem)), tcon_unwrap(extra_))
static inline int vec_concat_(struct vec_ *base, struct vec_ *extra)
{
	void *d = vec_append_space_(base, extra->used);
	if (!d)
		return -ENOMEM;
	memcpy(d, extra->data, extra->used);
	return 0;
}

#if 0
/* Does not work when @elem_ is an r-value as we cannot take the address of an r-value */
#define vec_append(vec_, elem_) vec_append_((vec_), &(elem_), tcon_sizeof((vec_), elem))
WARN_UNUSED_RESULT
static inline int vec_append_(struct vec_ *v, void *elem, size_t e_sz)
{
	void *l = vec_append_space_(v, e_sz);
	if (!l)
		return -1;

	memcpy(l, elem, e_sz);
	return 0;
}
#endif

/**
 * vec_index_or_null - return the Nth element of the vector, checking that is exists
 * @vec: &VEC(...): an initialized vector
 * @idx: size_t: the index of the element
 *
 * Returns a pointer to the element if it exists, or NULL if it does not.
 *
 * Example:
 *	assert(*vec_index_or_null(&nums, 1) == 44);
 *	assert(vec_index_or_null(&nums, 10) == NULL);
 */
#define vec_index_or_null(vec, idx) (tcon_cast_ptr((vec), elem, vec_index_or_null_(tcon_unwrap(vec), (idx), tcon_sizeof((vec), elem))))
static inline void *vec_index_or_null_(struct vec_ *v, size_t idx, size_t e_sz)
{
	size_t off = idx * e_sz;
	if (v->used < (off + e_sz))
		return NULL;
	return (char *)v->data + off;
}

/**
 * vec_next_or_null - return the next element if it is a member of vec, or NULL
 * @vec: &VEC(...): the vector
 * @elem_p: &elem_type: a pointer to an element in @vec
 *
 * Example:
 *	int *n = vec_next_or_null(&nums, l);
 *	assert(n);
 *	assert(*n == 12);
 */
#define vec_next_or_null(vec, elem_p) (tcon_cast_ptr((vec), elem, vec_next_or_null_(tcon_unwrap(vec), (elem_p), tcon_sizeof((vec), elem))))
static inline void *vec_next_or_null_(struct vec_ *v, void *elem, size_t e_sz)
{
	size_t off = (char *)elem - (char *)v->data;
	/* elem is not a member of this vector */
	assert(v->used >= (off + e_sz));

	off += e_sz;
	if (v->used < (off + e_sz))
		return NULL;
	return (char *)v->data + off;
}

/**
 * vec_index - index the vector, aborting when the index does not exist
 * @vec: &VEC(...): a vector
 * @idx: size_t: an index that exists in the vector
 *
 * Returns a pointer to the element of the vector.
 *
 * Example:
 *	assert(12 == *vec_index(&nums, 2));
 */
#define vec_index(vec, idx) (tcon_cast_ptr((vec), elem, vec_index_(tcon_unwrap(vec), (idx), tcon_sizeof((vec), elem))))
static inline void *vec_index_(struct vec_ *v, size_t idx, size_t e_sz)
{
	void *r = vec_index_or_null_(v, idx, e_sz);
	assert(r);
	return r;
}

/**
 * vec_offset - given a vector & an element, return the index of that element
 * @vec: &VEC(...): a vector
 * @elem: &elem_type: a pointer to an element in @vec
 *
 * Aborts if @elem is not a member of @vec.
 *
 * Example:
 *	int *thing = vec_index(&nums, 2);
 *	assert(2 == vec_offset(&nums, thing));
 */
#define vec_offset(vec_, elem_) vec_offset_(tcon_unwrap(tcon_check_ptr((vec_), elem, (elem_))), tcon_sizeof((vec_), elem), elem_)
static inline size_t vec_offset_(struct vec_ *v, size_t e_sz, void *elem)
{
	assert(elem >= v->data);
	assert(((char *)elem + e_sz) <= ((char *)v->data + v->used));
	return (elem - v->data) / e_sz;
}

/**
 * vec_len - return the number of elements in the vector
 * @vec: &VEC(...): a vector
 *
 * Return: 'size_t'
 *
 * Example:
 *	assert(3 == vec_len(&nums));
 */
#define vec_len(vec) vec_len_(tcon_unwrap(vec), tcon_sizeof((vec), elem))
static inline size_t vec_len_(struct vec_ *v, size_t e_sz)
{
	return v->used / e_sz;
}

/**
 * vec_data - return a pointer to the underlying data
 * @vec: &VEC(...): a vector
 *
 * Returns a pointer to the array of data in the vector.
 *
 * Example:
 *	assert(vec_data(&nums)[vec_len(&nums) - 1] == 11);
 */
#define vec_data(vec) (tcon_cast_ptr((vec), elem, tcon_unwrap(vec)->data))

/**
 * vec_for_each - iterate over pointers to elements of the vector
 * @vec: &VEC(...): vector to iterate over
 * @elem_p: &elem_type: a pointer to an element to be assigned to and used as
 *          the index
 *
 * Example:
 *	int *p;
 *	size_t s = 0;
 *	int acc = 0;
 *	vec_for_each(&nums, p) {
 *		s ++;
 *		acc += *p;
 *	}
 *	assert(s == vec_len(&nums));
 *	assert(acc == 66);
 *
 */
#define vec_for_each(vec, elem_p) \
	for(elem_p = vec_index_or_null(vec, 0); elem_p; elem_p = vec_next_or_null(vec, elem_p))

/**
 * vec_for_each_val - iterate over elements of the vector (values)
 * @vec: &VEC(...): vector to iterate over
 * @elem: elem_type: an element to assign to
 * @elem_p: &elem_type: a pointer to an element used as an index
 *
 * Example:
 *	s = 0;
 *	acc = 0;
 *	int v;
 *	vec_for_each_val(&nums, v, p) {
 *		s ++;
 *		acc += v;
 *	}
 *	assert(s == vec_len(&nums));
 *	assert(acc == 66);
 *
 */
#define vec_for_each_val(vec, elem_i, elem_p) \
	for(elem_p = vec_index_or_null(vec, 0);\
		(elem_p)?((elem_i) = *(elem_p), elem_p):elem_p; \
		elem_p = vec_next_or_null(vec, elem_p))

/**
 * vec_clear - remove all values from the vec
 * @vec: &VEC(...): a vector to clear
 *
 */
#define vec_clear(vec_) vec_clear_(tcon_unwrap(vec_))
static inline void vec_clear_(struct vec_ *v)
{
	v->used = 0;
}

/**
 * vec_drop - cleanup memory associated with a vec
 * @vec: &VEC(...): an initialized vector
 *
 * Example:
 *	vec_drop(&nums);
 */
#define vec_drop(vec) vec_drop_(tcon_unwrap(vec))
static inline void vec_drop_(struct vec_ *v)
{
	free(v->data);
}

#endif /* CCAN_VEC_H */
