/* GNU LGPL - see LICENSE file for details */
#ifndef CCAN_SIGNAL_H_
#define CCAN_SIGNAL_H_

#include <ccan/tlist2/tlist2.h>
#include <ccan/tcon/tcon.h>
#include <ccan/typesafe_cb/typesafe_cb.h>

#include <stdlib.h>

/* ret_type fn(ctx_type ctx, arg_type arg) */
typedef void (*signal_fn_erase_)(void *ctx, void *arg);

struct signal_connection {
    struct list_node node;
    signal_fn_erase_ fn;
    /* XXX: alignment should be specified */
    unsigned char ctx[];
};

struct signal_ {
    TLIST2(struct signal_connection, node) conns;
};

/**
 * SIGNAL - delare an object which can have multiple callbacks registered
 * @ret_type_: each callback may return a value. All for a single signal must
 *             return the same type. This is that type.
 * @arg_type_: when a signal is emitted, a value is provided to all the
 *             callbacks. This is the type of that value.
 *
 * Example:
 *      SIGNAL(int, double) notify_of_double_ret_int;
 */
#define SIGNAL(ret_type_, arg_type_) TCON_WRAP(struct signal_, ret_type_ ret; arg_type_ arg)

/**
 * SIGNAL_INIT - initializer for a SIGNAL(...)
 * @self_: name of the signal
 *
 * Example:
 *      SIGNAL(int, double) notify = SIGNAL_INIT(notify);
 *
 */
#define SIGNAL_INIT(self_) TCON_WRAP_INIT({.conns = TLIST2_INIT((self_)._base.conns)})

/**
 * signal_init - initialize a previously declared SIGNAL(...)
 * @sig: signal to initialize
 *
 * Example:
 *      SIGNAL(int, double) n2;
 *      signal_init(&n2);
 *
 */
#define signal_init(signal_) signal_init_(tcon_unwrap(signal_))
static inline void signal_init_(struct signal_ *sig)
{
    tlist2_init(&sig->conns);
}

/**
 * signal_connect - connect a callback & context (slot) to a signal
 * @signal_: &SIGNAL(...): a signal
 * @cb_: ret_type (*)(ctx_type *ctx, arg_type arg): the callback that is invoked
 * @ctx_: context, value is copied into the connection. A pointer to the value is provided to the callback
 *
 */
#define signal_connect(signal_, cb_, ctx_) \
    __extension__ ({ \
            __typeof__(ctx_) signal_connect_ctx_ = (ctx_); \
            signal_connect_(tcon_unwrap(signal_), \
                    typesafe_cb_cast( \
                        signal_fn_erase_, \
                        tcon_type((signal_), ret) (*) (__typeof__(&signal_connect_ctx_), tcon_type((signal_), arg)), \
                        cb_ \
                    ), &signal_connect_ctx_, sizeof(ctx_) \
            ); \
    })

static inline
struct signal_connection *signal_connect_(struct signal_ *sig, signal_fn_erase_ fn, void *this_ctx, size_t ctx_sz)
{
    /* allocate space for connection & ctx (as needed) */
    struct signal_connection *c = malloc(offsetof(struct signal_connection, ctx[ctx_sz]));
    if (!c)
        return NULL;

    /* store ctx */
    c->fn = fn;
    memcpy(c->ctx, this_ctx, ctx_sz);

    /* append conn to list */
    tlist2_add_tail(&sig->conns, c);
    return c;
}

/**
 * signal_call - call 1 signal connection
 * @signal_: &SIGNAL(...)
 * @signal_conn_: (struct signal_connection *), connection of the handler to call
 * @sig_val_: value to pass to the connected handler.
 */
#define signal_call(signal_, signal_conn_, sig_val_) \
    (((tcon_type((signal_), ret)(*)(void *ctx, tcon_type((signal_), arg)))\
      ((signal_conn_)->fn))(&(signal_conn_)->ctx, sig_val_))

/**
 * signal_for_each - send a value to each slot & provide the return value of each in turn
 * @signal_: &SIGNAL(...)
 * @sig_val_: `arg_type`, value to provide to each signal
 * @signal_conn_i_: a `struct signal_connection *` to track our iteration
 * @ret_i_: a `ret_type` which will be given the return value of each call
 *
 */
#define signal_for_each(signal_, sig_val_, signal_conn_i_, ret_i_) \
    for (signal_conn_i_ = tlist2_top(&tcon_unwrap(signal_)->conns); \
            signal_conn_i_ ? (ret_i_ = signal_call(signal_, signal_conn_i_, sig_val_), true) : false; \
            signal_conn_i_ = tlist2_next(&tcon_unwrap(signal_)->conns, signal_conn_i_))

/**
 * signal_emit - send a value to all slots, return the value of the last slot
 * @signal_: &SIGNAL(...)
 * @arg_val_: value to pass to the registered handlers
 *
 */
#define signal_emit(signal_, arg_val_) \
    __extension__ ({ \
        tcon_type((signal_), ret_type) signal_emit__ret_; \
        struct signal_connection *signal_emit__conn_; \
        signal_for_each((signal_), sig_val, signal_emit__conn_, signal_emit__ret_) \
            ; \
        signal_emit__ret_; \
    })

/**
 * signal_disconnect - remove a connection from a signal
 * @signal_: &SIGNAL(...): signal with connection
 * @connection_: struct signal_connection *: connection to remove
 *
 *
 */
#define signal_disconnect(signal_, connection_) tlist2_delfrom(&(signal_)->conns, connection_)

/**
 * signal_drop - destruct a signal
 *
 */
#define signal_drop(signal_) signal_drop_(tcon_unwrap(signal_))
static inline
void signal_drop_(struct signal_ *sig)
{
    struct signal_connection *c, *n;
    tlist2_for_each_safe(&sig->conns, c, n) {
        free(c);
    }
}

#endif
