#include "../mpack.h"
#include <unistd.h>

int main(void)
{
	int fds[2];
	pipe(fds);

	struct mpack_writer *enc = mpack_writer_fd_create(fds[1]);
	struct mpack_reader *dec = mpack_reader_fd_create(fds[0]);


	mpack_writer_fd_destroy(enc);
	mpack_reader_fd_destroy(dec);
	return 0;
}
