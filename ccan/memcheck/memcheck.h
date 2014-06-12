#ifndef CCAN_MEMCHECK_H_
#define CCAN_MEMCHECK_H_

/* clang address sanatizer */
#if defined(__has_feature)
#if __has_feature(address_sanitizer) || defined(__SANITIZE_ADDRESS__)

/* These definitions are usually provided through the
 * sanitizer/asan_interface.h header installed by ASan.
 * See https://code.google.com/p/address-sanitizer/wiki/ManualPoisoning
 */

void __asan_poison_memory_region(void const volatile *addr, size_t size);
void __asan_unpoison_memory_region(void const volatile *addr, size_t size);

#define MEMCHECK_MARK_NO_ACCESS(addr, size) \
	__asan_poison_memory_region((addr), (size))

#define MEMCHECK_MARK_UNDEFINED(addr, size) \
	__asan_unpoison_memory_region((addr), (size))

#define MEMCHECK_MARK_DEFINED(addr, size) \
	__asan_unpoison_memory_region((addr), (size))

#endif
#endif

/* valgrind */
/* See http://valgrind.org/docs/manual/mc-manual.html, "Client Requests",
 * defined in memcheck.h */
#define MEMCHECK_MARK_NO_ACCESS(addr, size) \
	VALGRIND_MAKE_MEM_NOACCESS(addr, size)
#define MEMCHECK_MARK_UNDEFINED(addr, size) \
	VALGRIND_MAKE_MEM_UNDEFINED(addr, size)
#define MEMCHECK_MARK_DEFINED(addr, size) \
	VALGRIND_MAKE_MEM_DEFINED(addr, size)

/* XXX: accessors:
 *	IS_DEFINED()
 *	IS_ACCESSABLE()
 *
 * XXX: Combination marks:
 *	MARK_DEFINED_IF_ACCESSABLE()
 *
 * XXX: mixing valgrind and asan?
 */


#endif /* CCAN_MEMCHECK_H_ */
