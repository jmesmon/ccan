/* Licenced under LGPLv3+ - see LICENSE file for details */
#include "mpack.h"

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

struct mpack_reader_fd {
	/* must be first */
	struct mpack_reader p;
	int fd;
};

static int mpack_reader_fd_read(mpack_reader *p, void *data, size_t len)
{
	struct mpack_reader_fd *f = (void *)p;
	return read(f->fd, data, len);
}

struct mpack_reader *mpack_reader_fd_create(int fd)
{
	struct mpack_reader_fd *m = malloc(sizeof(*m));
	if (!m)
		return NULL;

	*m = (struct mpack_reader_fd) {
		.fd = fd,
		.p.read = mpack_reader_fd_read,
	};

	return &m->p;
}

void mpack_reader_fd_destroy(struct mpack_reader *p)
{
	free(p);
}
