/* Licenced under LGPLv3+ - see LICENSE file for details */
#include "mpack.h"

#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

struct mpack_writer_fd {
	/* must be first */
	struct mpack_writer p;
	int fd;
};

static int mpack_writer_fd_write(mpack_writer *p, void *data, size_t len)
{
	struct mpack_writer_fd *f = (void *)p;
	return write(f->fd, data, len);
}

struct mpack_writer *mpack_writer_fd_create(int fd)
{
	struct mpack_writer_fd *m = malloc(sizeof(*m));
	if (!m)
		return NULL;

	*m = (struct mpack_writer_fd) {
		.fd = fd,
		.p.write = mpack_writer_fd_write,
	};

	return &m->p;
}

void mpack_writer_fd_destroy(struct mpack_writer *p)
{
	free(p);
}
