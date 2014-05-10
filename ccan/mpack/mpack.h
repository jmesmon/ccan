/* Licenced under LGPLv3+ - see LICENSE file for details */
#ifndef CCAN_MPACK_MPACK_H_
#define CCAN_MPACK_MPACK_H_
/* A minimal message pack encoder */

#include <stdint.h>
#include <stddef.h>

/*** encoder ***/
typedef struct mpack_writer {
	int (*write)(struct mpack_writer *p, void *data, size_t len);
} mpack_writer;

int mpack_pack_uint(mpack_writer *p, uintmax_t i);
int mpack_pack_int(mpack_writer *p, intmax_t i);
int mpack_pack_bytes(mpack_writer *p, void *data, size_t len);
int mpack_pack_array_start(mpack_writer *p, uint_least32_t count);

/** specific encoders **/
/* -> to a file descriptor, mpack_fd.c */
struct mpack_writer *mpack_writer_fd_create(int fd);
void mpack_writer_fd_destroy(struct mpack_writer *p);

/*** decoder ***/
enum mpack_type {
	MPACK_TYPE_UINT,
	MPACK_TYPE_INT,
	MPACK_TYPE_BYTES,
	MPACK_TYPE_ARRAY,
};

struct mpack_obj {
	int ref;
	enum mpack_type type;
	union {
		uintmax_t u;
		intmax_t i;
		struct mpack_bytes {
			uint8_t *data;
			size_t len;
		} bytes;
		size_t array_elems;
	} val;
};

void mpack_obj_up(struct mpack_obj *o);
void mpack_obj_down(struct mpack_obj *o);

typedef struct mpack_reader {
	int (*read)(struct mpack_reader *p, void *data, size_t len);
} mpack_reader;

/*
 * returns a mpack_obj with a single reference count, or NULL on error and EOF
 */
struct mpack_obj *mpack_reader_next(struct mpack_reader *p);

/** specific readers **/
/* from a file descriptor, mpack_decode_fd.c */
struct mpack_reader *mpack_reader_fd_create(int fd);
void mpack_reader_fd_destroy(struct mpack_reader *p);

#endif
