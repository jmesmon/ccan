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
 * SIGNAL1 - delare an object which can have multiple callbacks registered
 * @ret_type_: each callback may return a value. All for a single signal must
 *             return the same type. This is that type.
 * @arg_type_: when a signal is emitted, a value is provided to all the
 *             callbacks. This is the type of that value.
 *
 * Example:
 *      SIGNAL1(int, double) notify_of_double_ret_int;
 */
#define SIGNAL1(ret_type_, arg_type_) TCON_WRAP(struct signal_, \
        ret_type_ (*fn_ret)(void); \
        ret_type_ (*fn_partial)(void *, arg_type_); \
        arg_type_ arg)

/**
 * SIGNAL0 - delare an object which can have multiple callbacks registered
 * @ret_type_: each callback may return a value. All for a single signal must
 *             return the same type. This is that type.
 *
 * Example:
 *      SIGNAL0(int) notify_ret_int;
 */
#define SIGNAL0(ret_type_) TCON_WRAP(struct signal_, \
        ret_type_ (*fn_ret)(void); \
        ret_type_ (*fn_partial)(void *))

/**
 * SIGNAL_INIT - initializer for a SIGNAL(...)
 * @self_: name of the signal
 *
 * Example:
 *      SIGNAL1(int, double) notify = SIGNAL_INIT(notify);
 *
 */
#define SIGNAL_INIT(self_) TCON_WRAP_INIT({.conns = TLIST2_INIT((self_)._base.conns)})

/**
 * signal_init - initialize a previously declared SIGNAL(...)
 * @sig: signal to initialize
 *
 * Example:
 *      SIGNAL1(int, double) n2;
 *      signal_init(&n2);
 *
 */
#define signal_init(signal_) signal_init_(tcon_unwrap(signal_))
static inline void signal_init_(struct signal_ *sig)
{
    tlist2_init(&sig->conns);
}

#define signal_n_connect(signal_, cb_, ctx_, ...) \
    __extension__ ({ \
            __typeof__(ctx_) signal_connect_ctx_ = (ctx_); \
            signal_connect_(tcon_unwrap(signal_), \
                    typesafe_cb_cast( \
                        signal_fn_erase_, \
                        tcon_ret_type((signal_), fn_ret) (*) (__typeof__(&signal_connect_ctx_), ## __VA_ARGS__), \
                        cb_ \
                    ), &signal_connect_ctx_, sizeof(ctx_) \
            ); \
    })

/**
 * signal1_connect - connect a callback & context (slot) to a signal
 * @signal_: &SIGNAL(...): a signal
 * @cb_: ret_type (*)(ctx_type *ctx, arg_type arg): the callback that is invoked
 * @ctx_: context, value is copied into the connection. A pointer to the value is provided to the callback
 *
 * Note: use of expression statement needed to generate lvalue from a potential rvalue.
 */
#define signal1_connect(signal_, cb_, ctx_) \
    signal_n_connect((signal_), (cb_), (ctx_), tcon_type((signal_), arg))

/**
 * signal0_connect - connect a callback & context (slot) to a signal
 * @signal_: &SIGNAL(...): a signal
 * @cb_: ret_type (*)(ctx_type *ctx, arg_type arg): the callback that is invoked
 * @ctx_: context, value is copied into the connection. A pointer to the value is provided to the callback
 *
 * Note: use of expression statement needed to generate lvalue from a potential rvalue.
 */
#define signal0_connect(signal_, cb_, ctx_) \
    signal_n_connect((signal_), (cb_), (ctx_))

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
 * @sig_val: value to pass to callbacks
 */
#define signal_call(signal_, signal_conn_, ...) \
    (((tcon_type((signal_), fn_partial))\
      ((signal_conn_)->fn))(&(signal_conn_)->ctx, ## __VA_ARGS__))

/**
 * signal_for_each - send a value to each slot & provide the return value of each in turn
 * @signal_: &SIGNAL(...)
 * @signal_conn_i_: a `struct signal_connection *` to track our iteration
 * @ret_i_: a `ret_type` which will be given the return value of each call
 * @sig_val_: `arg_type`, value to provide to each signal
 *
 */
#define signal_for_each(signal_, signal_conn_i_, ret_i_, ...) \
    for (signal_conn_i_ = tlist2_top(&tcon_unwrap(signal_)->conns); \
            signal_conn_i_ ? (ret_i_ = signal_call(signal_, signal_conn_i_, ## __VA_ARGS__), true) : false; \
            signal_conn_i_ = tlist2_next(&tcon_unwrap(signal_)->conns, signal_conn_i_))

/**
 * signal_for_each_void - send a value to each slot
 * @signal_: &SIGNAL(...)
 * @signal_conn_i_: a `struct signal_connection *` to track our iteration
 * @sig_val_: `arg_type`, value to provide to each signal
 *
 */
#define signal_for_each_void(signal_, signal_conn_i_, ...) \
    for (signal_conn_i_ = tlist2_top(&tcon_unwrap(signal_)->conns); \
            signal_conn_i_ ? (signal_call(signal_, signal_conn_i_, ## __VA_ARGS__), true) : false; \
            signal_conn_i_ = tlist2_next(&tcon_unwrap(signal_)->conns, signal_conn_i_))

#define signal_if_void(expr_, if_void, if_non_void) \
    __builtin_choose_expr(__builtin_types_compatible_p(void, __typeof__(expr_)), \
            (if_void), \
            (if_non_void) \
    )

#define type_non_void __typeof__(signal_if_void
#define signal_expr_non_void(expr_) signal_if_void((expr_), (expr_, NULL), (expr_))

/**
 * signal_emit - send a value to all slots, return the value of the last slot
 * @signal_: &SIGNAL(...)
 * @arg_val_: value to pass to the registered handlers
 *
 */
#define signal_emit(signal_, ...) \
    __builtin_choose_expr( \
        __builtin_types_compatible_p(__typeof__((signal_)->_tcon[0].fn_ret), void(*)(void)), \
            __extension__ ({ \
                struct signal_connection *signal_emit__conn_; \
                signal_for_each_void((signal_), signal_emit__conn_, ## __VA_ARGS__); \
            }), \
            __extension__ ({ \
                tcon_ret_type((signal_), fn_ret) signal_emit__ret_; \
                struct signal_connection *signal_emit__conn_; \
                signal_for_each((signal_), signal_emit__conn_, signal_emit__ret_, ## __VA_ARGS__) \
                    ; \
                signal_emit__ret_; \
            }) \
    )

/**
 * signal_disconnect - remove a connection from a signal
 * @signal_: &SIGNAL(...): signal with connection
 * @connection_: struct signal_connection *: connection to remove
 *
 *
 */
#define signal_disconnect(signal_, connection_) tlist2_delfrom(&(signal_)->conns, connection_)

/**
 * signal_drop - destruct a signal, freeing backing memory
 * @signal_: the signal to drop
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
