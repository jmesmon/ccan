#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

struct imap_client;
typedef int (*imap_client_cb)(struct imap_client *c, const char *cmd, size_t cmd_len);

struct imap_session {
	uint32_t uidvalidity;
};

/*
 * Message Attributes
 *  - Flags
 *	\Seen
 *	\Answered
 *	\Flagged
 *	\Deleted
 *	\Draft
 *	\Recent
 *	[and any user defined flags]
 *  - date & time reflecting message reception
 *  - size
 *  - envelope structure
 *  - body structure
 */

/*
 * XXX: Consider unifying
 */
struct imap_server_message;
struct imap_client_message;

/*
 * XXX: from the sd-bus impl, purpose unclear
 */
struct imap_client_slot;

/*
 * XXX: consider if we want to allow non-fd interfaces.
 * XXX: if anything tcp talks IMAP, we probably will
 */
struct imap_client {
	int fd;

	/* XXX: consider if we can avoid this much bloat in the client context */
	char input[1024];
	size_t input_offs;

	/* we need a timeout tracking structure here */
	uint64_t timeout;

	imap_client_cb untagged_cb;
	imap_client_cb tagged_cb;
};

/*
 * Should be called when the imap_client fd indicates it is ready for use.  In
 * the case where it is called when the fd is un-ready, this functions will
 * block if the fd is set to blocking, and will not block in the case where the
 * fd is set to NONBLOCKING.
 */
int imap_client_process(struct imap_client *c, struct imap_server_message **msg)
{
	ssize_t r = read(c->fd, c->input + c->input_offs, sizeof(c->input) - c->input_offs);

	if (r < 0) {
		/* whoops */
	}

	return 0;
}

/*
 * Generate a tag to prefix a Client command
 */
size_t imap_client_tag(struct imap_client *c, char *buf, size_t len)
{

}

/*
 * XXX: do we want to push the resolve into this, or force it somewhere else?
 *
 * XXX: in the case where we need to reconnect, we also should consider re-resolving, in which case we seem to require host/serv
 * XXX: could provide a callback to preform the actual resolution to avoid 
 */
void imap_client_init(struct imap_client *c, const char *host, const char *serv)
{

}
