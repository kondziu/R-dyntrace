#ifndef __DYNTRACE_H__
#define __DYNTRACE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <config.h>

#define HAVE_DECL_SIZE_MAX 1
#define R_USE_SIGNALS 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_LIMITS_H 1
#include <Defn.h>

#ifdef ENABLE_DYNTRACE

#define DYNTRACE_PROBE_HEADER(probe_name)                                      \
    if (dyntrace_active_dyntracer != NULL &&                                   \
        dyntrace_active_dyntracer->probe_name != NULL &&                       \
        dyntrace_probe_is_enabled(probe_name) &&                               \
        !dyntrace_is_priviliged_mode()) {                                      \
        CHECK_REENTRANCY(probe_name);                                          \
        dyntrace_active_dyntracer_probe_name = #probe_name;                    \
        dyntrace_disable_garbage_collector();

#define DYNTRACE_PROBE_FOOTER(probe_name)                                      \
    dyntrace_reinstate_garbage_collector();                                    \
    dyntrace_active_dyntracer_probe_name = NULL;                               \
    }

#define CHECK_REENTRANCY(probe_name)                                           \
    if (dyntrace_check_reentrancy) {                                           \
        if (dyntrace_active_dyntracer_probe_name != NULL) {                    \
            dyntrace_log_error("[NESTED HOOK EXECUTION] - %s triggers %s",     \
                               dyntrace_active_dyntracer_probe_name,           \
                               #probe_name);                                   \
        }                                                                      \
    }

#define DYNTRACE_PROBE_DYNTRACE_ENTRY(expression, environment)                 \
    DYNTRACE_PROBE_HEADER(probe_dyntrace_entry);                               \
    PROTECT(expression);                                                       \
    PROTECT(environment);                                                      \
    dyntrace_active_dyntracer->probe_dyntrace_entry(dyntrace_active_dyntracer, \
                                                    expression, environment);  \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_dyntrace_entry);

#define DYNTRACE_PROBE_DYNTRACE_EXIT(expression, environment, result, error)   \
    DYNTRACE_PROBE_HEADER(probe_dyntrace_exit);                                \
    PROTECT(expression);                                                       \
    PROTECT(environment);                                                      \
    PROTECT(result);                                                           \
    dyntrace_active_dyntracer->probe_dyntrace_exit(                            \
        dyntrace_active_dyntracer, expression, environment, result, error);    \
    UNPROTECT(3);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_dyntrace_exit);

#define DYNTRACE_SHOULD_PROBE(probe_name)                                      \
    (dyntrace_active_dyntracer->probe_name != NULL)

#define DYNTRACE_PROBE_CLOSURE_ENTRY(call, op, args, rho)                      \
    DYNTRACE_PROBE_HEADER(probe_closure_entry);                                \
    PROTECT(call);                                                             \
    PROTECT(op);                                                               \
    PROTECT(args);                                                             \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_closure_entry(dyntrace_active_dyntracer,  \
                                                   call, op, args, rho);       \
    UNPROTECT(4);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_closure_entry);

#define DYNTRACE_PROBE_CLOSURE_EXIT(call, op, args, rho, retval)               \
    DYNTRACE_PROBE_HEADER(probe_closure_exit);                                 \
    PROTECT(call);                                                             \
    PROTECT(op);                                                               \
    PROTECT(args);                                                             \
    PROTECT(rho);                                                              \
    PROTECT(retval);                                                           \
    dyntrace_active_dyntracer->probe_closure_exit(                             \
        dyntrace_active_dyntracer, call, op, args, rho, retval);               \
    UNPROTECT(5);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_closure_exit);

#define DYNTRACE_PROBE_BUILTIN_ENTRY(call, op, args, rho)                      \
    DYNTRACE_PROBE_HEADER(probe_builtin_entry);                                \
    PROTECT(call);                                                             \
    PROTECT(op);                                                               \
    PROTECT(args);                                                             \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_builtin_entry(dyntrace_active_dyntracer,  \
                                                   call, op, args, rho);       \
    UNPROTECT(4);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_builtin_entry);

#define DYNTRACE_PROBE_BUILTIN_EXIT(call, op, args, rho, retval)               \
    DYNTRACE_PROBE_HEADER(probe_builtin_exit);                                 \
    PROTECT(call);                                                             \
    PROTECT(op);                                                               \
    PROTECT(args);                                                             \
    PROTECT(rho);                                                              \
    PROTECT(retval);                                                           \
    dyntrace_active_dyntracer->probe_builtin_exit(                             \
        dyntrace_active_dyntracer, call, op, args, rho, retval);               \
    UNPROTECT(5);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_builtin_exit);

#define DYNTRACE_PROBE_SPECIAL_ENTRY(call, op, args, rho)                      \
    DYNTRACE_PROBE_HEADER(probe_special_entry);                                \
    PROTECT(call);                                                             \
    PROTECT(op);                                                               \
    PROTECT(args);                                                             \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_special_entry(dyntrace_active_dyntracer,  \
                                                   call, op, args, rho);       \
    UNPROTECT(4);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_special_entry);

#define DYNTRACE_PROBE_SPECIAL_EXIT(call, op, args, rho, retval)               \
    DYNTRACE_PROBE_HEADER(probe_special_exit);                                 \
    PROTECT(call);                                                             \
    PROTECT(op);                                                               \
    PROTECT(args);                                                             \
    PROTECT(rho);                                                              \
    PROTECT(retval);                                                           \
    dyntrace_active_dyntracer->probe_special_exit(                             \
        dyntrace_active_dyntracer, call, op, args, rho, retval);               \
    UNPROTECT(5);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_special_exit);

#define DYNTRACE_PROBE_PROMISE_FORCE_ENTRY(promise)                            \
    DYNTRACE_PROBE_HEADER(probe_promise_force_entry);                          \
    PROTECT(promise);                                                          \
    dyntrace_active_dyntracer->probe_promise_force_entry(                      \
        dyntrace_active_dyntracer, promise);                                   \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_force_entry);

#define DYNTRACE_PROBE_PROMISE_FORCE_EXIT(promise)                             \
    DYNTRACE_PROBE_HEADER(probe_promise_force_exit);                           \
    PROTECT(promise);                                                          \
    dyntrace_active_dyntracer->probe_promise_force_exit(                       \
        dyntrace_active_dyntracer, promise);                                   \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_force_exit);

#define DYNTRACE_PROBE_PROMISE_VALUE_LOOKUP(promise)                           \
    DYNTRACE_PROBE_HEADER(probe_promise_value_lookup);                         \
    PROTECT(promise);                                                          \
    dyntrace_active_dyntracer->probe_promise_value_lookup(                     \
        dyntrace_active_dyntracer, promise);                                   \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_value_lookup);

#define DYNTRACE_PROBE_PROMISE_VALUE_ASSIGN(promise, value)                    \
    DYNTRACE_PROBE_HEADER(probe_promise_value_assign);                         \
    PROTECT(promise);                                                          \
    PROTECT(value);                                                            \
    dyntrace_active_dyntracer->probe_promise_value_assign(                     \
        dyntrace_active_dyntracer, promise, value);                            \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_value_assign);

#define DYNTRACE_PROBE_PROMISE_EXPRESSION_LOOKUP(promise)                      \
    DYNTRACE_PROBE_HEADER(probe_promise_expression_lookup);                    \
    PROTECT(promise);                                                          \
    dyntrace_active_dyntracer->probe_promise_expression_lookup(                \
        dyntrace_active_dyntracer, promise);                                   \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_expression_lookup);

#define DYNTRACE_PROBE_PROMISE_EXPRESSION_ASSIGN(promise, expression)          \
    DYNTRACE_PROBE_HEADER(probe_promise_expression_assign);                    \
    PROTECT(promise);                                                          \
    PROTECT(expression);                                                       \
    dyntrace_active_dyntracer->probe_promise_expression_assign(                \
        dyntrace_active_dyntracer, promise, expression);                       \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_expression_assign);

#define DYNTRACE_PROBE_PROMISE_ENVIRONMENT_LOOKUP(promise)                     \
    DYNTRACE_PROBE_HEADER(probe_promise_environment_lookup);                   \
    PROTECT((promise));                                                        \
    dyntrace_active_dyntracer->probe_promise_environment_lookup(               \
        dyntrace_active_dyntracer, promise);                                   \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_environment_lookup);

#define DYNTRACE_PROBE_PROMISE_ENVIRONMENT_ASSIGN(promise, environment)        \
    DYNTRACE_PROBE_HEADER(probe_promise_environment_assign);                   \
    PROTECT(promise);                                                          \
    PROTECT(environment);                                                      \
    dyntrace_active_dyntracer->probe_promise_environment_assign(               \
        dyntrace_active_dyntracer, promise, environment);                      \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_promise_environment_assign);

#define DYNTRACE_PROBE_EVAL_ENTRY(expression, rho)                             \
    DYNTRACE_PROBE_HEADER(probe_eval_entry);                                   \
    PROTECT(expression);                                                       \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_eval_entry(dyntrace_active_dyntracer,     \
                                                expression, rho);              \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_eval_entry);

#define DYNTRACE_PROBE_EVAL_EXIT(expression, rho, return_value)                \
    DYNTRACE_PROBE_HEADER(probe_eval_exit);                                    \
    PROTECT(expression);                                                       \
    PROTECT(rho);                                                              \
    PROTECT(return_value);                                                     \
    dyntrace_active_dyntracer->probe_eval_exit(dyntrace_active_dyntracer,      \
                                               expression, rho, return_value); \
    UNPROTECT(3);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_eval_exit);

#define DYNTRACE_PROBE_GC_ENTRY(size_needed)                                   \
    DYNTRACE_PROBE_HEADER(probe_gc_entry);                                     \
    dyntrace_active_dyntracer->probe_gc_entry(dyntrace_active_dyntracer,       \
                                              size_needed);                    \
    DYNTRACE_PROBE_FOOTER(probe_gc_entry);

#define DYNTRACE_PROBE_GC_EXIT(gc_count)                                       \
    DYNTRACE_PROBE_HEADER(probe_gc_exit);                                      \
    dyntrace_active_dyntracer->probe_gc_exit(dyntrace_active_dyntracer,        \
                                             gc_count);                        \
    DYNTRACE_PROBE_FOOTER(probe_gc_exit);

#define DYNTRACE_PROBE_GC_UNMARK(object)                                       \
    DYNTRACE_PROBE_HEADER(probe_gc_unmark);                                    \
    PROTECT(object);                                                           \
    dyntrace_active_dyntracer->probe_gc_unmark(dyntrace_active_dyntracer,      \
                                               object);                        \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_gc_unmark);

#define DYNTRACE_PROBE_GC_ALLOCATE(object)                                     \
    DYNTRACE_PROBE_HEADER(probe_gc_allocate);                                  \
    PROTECT(object);                                                           \
    dyntrace_active_dyntracer->probe_gc_allocate(dyntrace_active_dyntracer,    \
                                                 object);                      \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_gc_allocate);

#define DYNTRACE_PROBE_CONTEXT_ENTRY(context)                                  \
    DYNTRACE_PROBE_HEADER(probe_context_entry);                                \
    dyntrace_active_dyntracer->probe_context_entry(dyntrace_active_dyntracer,  \
                                                   context);                   \
    DYNTRACE_PROBE_FOOTER(probe_context_entry);

#define DYNTRACE_PROBE_CONTEXT_EXIT(context)                                   \
    DYNTRACE_PROBE_HEADER(probe_context_exit);                                 \
    dyntrace_active_dyntracer->probe_context_exit(dyntrace_active_dyntracer,   \
                                                  context);                    \
    DYNTRACE_PROBE_FOOTER(probe_context_end);

#define DYNTRACE_PROBE_CONTEXT_JUMP(context, return_value, restart)            \
    DYNTRACE_PROBE_HEADER(probe_context_jump);                                 \
    PROTECT(return_value);                                                     \
    dyntrace_active_dyntracer->probe_context_jump(                             \
        dyntrace_active_dyntracer, context, return_value, restart);            \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_context_jump);

#define DYNTRACE_PROBE_S3_GENERIC_ENTRY(generic, object)                       \
    DYNTRACE_PROBE_HEADER(probe_S3_generic_entry);                             \
    PROTECT(object);                                                           \
    dyntrace_active_dyntracer->probe_S3_generic_entry(                         \
        dyntrace_active_dyntracer, generic, object);                           \
    UNPROTECT(1);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_S3_generic_entry);

#define DYNTRACE_PROBE_S3_GENERIC_EXIT(generic, object, retval)                \
    DYNTRACE_PROBE_HEADER(probe_S3_generic_exit);                              \
    PROTECT(object);                                                           \
    PROTECT(retval);                                                           \
    dyntrace_active_dyntracer->probe_S3_generic_exit(                          \
        dyntrace_active_dyntracer, generic, object, retval);                   \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_S3_generic_exit);

#define DYNTRACE_PROBE_S3_DISPATCH_ENTRY(generic, cls, method, object)         \
    DYNTRACE_PROBE_HEADER(probe_S3_dispatch_entry);                            \
    PROTECT(method);                                                           \
    PROTECT(object);                                                           \
    dyntrace_active_dyntracer->probe_S3_dispatch_entry(                        \
        dyntrace_active_dyntracer, generic, cls, method, object);              \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_S3_dispatch_entry);

#define DYNTRACE_PROBE_S3_DISPATCH_EXIT(generic, cls, method, object, retval)  \
    DYNTRACE_PROBE_HEADER(probe_S3_dispatch_exit);                             \
    PROTECT(method);                                                           \
    PROTECT(object);                                                           \
    PROTECT(retval);                                                           \
    dyntrace_active_dyntracer->probe_S3_dispatch_exit(                         \
        dyntrace_active_dyntracer, generic, cls, method, object, retval);      \
    UNPROTECT(3);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_S3_dispatch_exit);

#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_DEFINE(symbol, value, rho)         \
    DYNTRACE_PROBE_HEADER(probe_environment_variable_define);                  \
    PROTECT(symbol);                                                           \
    PROTECT(value);                                                            \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_environment_variable_define(              \
        dyntrace_active_dyntracer, symbol, value, rho);                        \
    UNPROTECT(3);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_environment_variable_define);

#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_ASSIGN(symbol, value, rho)         \
    DYNTRACE_PROBE_HEADER(probe_environment_variable_assign);                  \
    PROTECT(symbol);                                                           \
    PROTECT(value);                                                            \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_environment_variable_assign(              \
        dyntrace_active_dyntracer, symbol, value, rho);                        \
    UNPROTECT(3);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_environment_variable_assign);

#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_REMOVE(symbol, rho)                \
    DYNTRACE_PROBE_HEADER(probe_environment_variable_remove);                  \
    PROTECT(symbol);                                                           \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_environment_variable_remove(              \
        dyntrace_active_dyntracer, symbol, rho);                               \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_environment_variable_remove);

#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_LOOKUP(symbol, value, rho)         \
    DYNTRACE_PROBE_HEADER(probe_environment_variable_lookup);                  \
    PROTECT(symbol);                                                           \
    PROTECT(value);                                                            \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_environment_variable_lookup(              \
        dyntrace_active_dyntracer, symbol, value, rho);                        \
    UNPROTECT(3);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_environment_variable_lookup);

#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_EXISTS(symbol, rho)                \
    DYNTRACE_PROBE_HEADER(probe_environment_variable_exists);                  \
    PROTECT(symbol);                                                           \
    PROTECT(rho);                                                              \
    dyntrace_active_dyntracer->probe_environment_variable_exists(              \
        dyntrace_active_dyntracer, symbol, rho);                               \
    UNPROTECT(2);                                                              \
    DYNTRACE_PROBE_FOOTER(probe_environment_variable_exists);

#define dyntrace_probe_is_enabled(probe_name)                                  \
    (dyntrace_##probe_name##_disabled == 0)

#define dyntrace_disable_probe(probe_name) dyntrace_##probe_name##_disabled = 1;

#define dyntrace_enable_probe(probe_name) dyntrace_##probe_name##_disabled = 0;

#else

#define DYNTRACE_PROBE_DYNTRACE_ENTRY(expression, environment)
#define DYNTRACE_PROBE_DYNTRACE_EXIT(expression, environment, result, error)

#define DYNTRACE_PROBE_CLOSURE_ENTRY(call, op, args, rho)
#define DYNTRACE_PROBE_CLOSURE_EXIT(call, op, args, rho, retval)
#define DYNTRACE_PROBE_BUILTIN_ENTRY(call, op, args, rho)
#define DYNTRACE_PROBE_BUILTIN_EXIT(call, op, args, rho, retval)
#define DYNTRACE_PROBE_SPECIAL_ENTRY(call, op, args, rho)
#define DYNTRACE_PROBE_SPECIAL_EXIT(call, op, args, rho, retval)

#define DYNTRACE_PROBE_PROMISE_FORCE_ENTRY(promise)
#define DYNTRACE_PROBE_PROMISE_FORCE_EXIT(promise)
#define DYNTRACE_PROBE_PROMISE_VALUE_LOOKUP(promise)
#define DYNTRACE_PROBE_PROMISE_VALUE_ASSIGN(promise, value)
#define DYNTRACE_PROBE_PROMISE_EXPRESSION_LOOKUP(promise)
#define DYNTRACE_PROBE_PROMISE_EXPRESSION_ASSIGN(promise, expression)
#define DYNTRACE_PROBE_PROMISE_ENVIRONMENT_LOOKUP(promise)
#define DYNTRACE_PROBE_PROMISE_ENVIRONMENT_ASSIGN(promise, environment)

#define DYNTRACE_PROBE_EVAL_ENTRY(expression, rho)
#define DYNTRACE_PROBE_EVAL_EXIT(expression, rho, return_value)

#define DYNTRACE_PROBE_GC_ENTRY(size_needed)
#define DYNTRACE_PROBE_GC_EXIT(gc_count)
#define DYNTRACE_PROBE_GC_UNMARK(object)
#define DYNTRACE_PROBE_GC_ALLOCATE(object)

#define DYNTRACE_PROBE_CONTEXT_ENTRY(context)
#define DYNTRACE_PROBE_CONTEXT_EXIT(context)
#define DYNTRACE_PROBE_CONTEXT_JUMP(context, return_value, restart)

#define DYNTRACE_PROBE_S3_GENERIC_ENTRY(generic, object)
#define DYNTRACE_PROBE_S3_GENERIC_EXIT(generic, object, retval)
#define DYNTRACE_PROBE_S3_DISPATCH_ENTRY(generic, cls, method, object)
#define DYNTRACE_PROBE_S3_DISPATCH_EXIT(generic, cls, method, object, retval)

#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_DEFINE(symbol, value, rho)
#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_ASSIGN(symbol, value, rho)
#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_REMOVE(symbol, rho)
#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_LOOKUP(symbol, value, rho)
#define DYNTRACE_PROBE_ENVIRONMENT_VARIABLE_EXISTS(symbol, rho)

#define dyntrace_probe_is_enabled(probe_name)
#define dyntrace_disable_probe(probe_name)
#define dyntrace_enable_probe(probe_name)
#endif

/* ----------------------------------------------------------------------------
   DYNTRACE TYPE DEFINITIONS
---------------------------------------------------------------------------- */
typedef struct dyntracer_t dyntracer_t;

struct dyntracer_t {

    /***************************************************************************
    Fires when the tracer starts. Not an interpreter probe.
    ***************************************************************************/
    void (*probe_dyntrace_entry)(dyntracer_t *dyntracer, SEXP expression,
                                 SEXP environment);

    /***************************************************************************
    Fires when the tracer ends. Not an interpreter probe.
    ***************************************************************************/
    void (*probe_dyntrace_exit)(dyntracer_t *dyntracer, SEXP expression,
                                SEXP environment, SEXP result, int error);

    /***************************************************************************
    Fires on every closure call.
    ***************************************************************************/
    void (*probe_closure_entry)(dyntracer_t *dyntracer, const SEXP call,
                                const SEXP op, const SEXP args, const SEXP rho);

    /***************************************************************************
    Fires after every R closure call.
    ***************************************************************************/
    void (*probe_closure_exit)(dyntracer_t *dyntracer, const SEXP call,
                               const SEXP op, const SEXP args, const SEXP rho,
                               const SEXP retval);

    /***************************************************************************
    Fires on every builtin call.
    ***************************************************************************/
    void (*probe_builtin_entry)(dyntracer_t *dyntracer, const SEXP call,
                                const SEXP op, const SEXP args, const SEXP rho);

    /***************************************************************************
    Fires after every builtin call.
    ***************************************************************************/
    void (*probe_builtin_exit)(dyntracer_t *dyntracer, const SEXP call,
                               const SEXP op, const SEXP args, const SEXP rho,
                               const SEXP retval);

    /***************************************************************************
    Fires on every special call.
    ***************************************************************************/
    void (*probe_special_entry)(dyntracer_t *dyntracer, const SEXP call,
                                const SEXP op, const SEXP args, const SEXP rho);

    /***************************************************************************
    Fires after every special call.
    ***************************************************************************/
    void (*probe_special_exit)(dyntracer_t *dyntracer, const SEXP call,
                               const SEXP op, const SEXP args, const SEXP rho,
                               const SEXP retval);

    /***************************************************************************
    Fires when a promise expression is evaluated.
    ***************************************************************************/
    void (*probe_promise_force_entry)(dyntracer_t *dyntracer,
                                      const SEXP promise);

    /***************************************************************************
    Fires after a promise expression is evaluated.
    ***************************************************************************/
    void (*probe_promise_force_exit)(dyntracer_t *dyntracer,
                                     const SEXP promise);

    /***************************************************************************
    Fires when the promise value is looked up.
    ***************************************************************************/
    void (*probe_promise_value_lookup)(dyntracer_t *dyntracer,
                                       const SEXP promise);

    /***************************************************************************
    Fires when the promise value is assigned.
    ***************************************************************************/
    void (*probe_promise_value_assign)(dyntracer_t *dyntracer,
                                       const SEXP promise, const SEXP value);

    /***************************************************************************
    Fires when the promise expression is looked up.
    ***************************************************************************/
    void (*probe_promise_expression_lookup)(dyntracer_t *dyntracer,
                                            const SEXP promise);

    /***************************************************************************
    Fires when the promise expression is assigned.
    ***************************************************************************/
    void (*probe_promise_expression_assign)(dyntracer_t *dyntracer,
                                            const SEXP promise,
                                            const SEXP expression);

    /***************************************************************************
    Fires when the promise environment is looked up.
    ***************************************************************************/
    void (*probe_promise_environment_lookup)(dyntracer_t *dyntracer,
                                             const SEXP promise);

    /***************************************************************************
    Fires when the promise environment is assigned.
    ***************************************************************************/
    void (*probe_promise_environment_assign)(dyntracer_t *dyntracer,
                                             const SEXP promise,
                                             const SEXP environment);

    /***************************************************************************
    Fires when the eval function is entered.
    ***************************************************************************/
    void (*probe_eval_entry)(dyntracer_t *dyntracer, const SEXP expression,
                             const SEXP rho);

    /***************************************************************************
    Fires when the eval function is exited.
    ***************************************************************************/
    void (*probe_eval_exit)(dyntracer_t *dyntracer, const SEXP expression,
                            const SEXP rho, SEXP return_value);

    /***************************************************************************
    Fires when the garbage collector is entered.
    ***************************************************************************/
    void (*probe_gc_entry)(dyntracer_t *dyntracer, const size_t size_needed);

    /***************************************************************************
    Fires when the garbage collector is exited.
    ***************************************************************************/
    void (*probe_gc_exit)(dyntracer_t *dyntracer, int gc_count);

    /***************************************************************************
    Fires when an object gets garbage collected.
    ***************************************************************************/
    void (*probe_gc_unmark)(dyntracer_t *dyntracer, const SEXP object);

    /***************************************************************************
    Fires when an object gets allocated.
    ***************************************************************************/
    void (*probe_gc_allocate)(dyntracer_t *dyntracer, const SEXP object);

    /***************************************************************************
    Fires when a new context is entered.
    ***************************************************************************/
    void (*probe_context_entry)(dyntracer_t *dyntracer, const RCNTXT *context);

    /***************************************************************************
    Fires when a context is exited.
    ***************************************************************************/
    void (*probe_context_exit)(dyntracer_t *dyntracer, const RCNTXT *context);

    /***************************************************************************
    Fires when the interpreter is about to longjump into a different context.
    ***************************************************************************/
    void (*probe_context_jump)(dyntracer_t *dyntracer, const RCNTXT *context,
                               const SEXP return_value, int restart);

    /***************************************************************************
    Fires when a generic S3 function is entered.
    ***************************************************************************/
    void (*probe_S3_generic_entry)(dyntracer_t *dyntracer, const char *generic,
                                   const SEXP object);

    /***************************************************************************
    Fires when a generic S3 function is exited.
    ***************************************************************************/
    void (*probe_S3_generic_exit)(dyntracer_t *dyntracer, const char *generic,
                                  const SEXP object, const SEXP retval);

    /***************************************************************************
    Fires when a S3 function is entered.
    ***************************************************************************/
    void (*probe_S3_dispatch_entry)(dyntracer_t *dyntracer, const char *generic,
                                    const char *cls, const SEXP method,
                                    const SEXP object);

    /***************************************************************************
    Fires when a S3 function is exited.
    ***************************************************************************/
    void (*probe_S3_dispatch_exit)(dyntracer_t *dyntracer, const char *generic,
                                   const char *cls, const SEXP method,
                                   const SEXP object, const SEXP retval);

    /***************************************************************************
    Fires when a variable is defined in an environment.
    ***************************************************************************/
    void (*probe_environment_variable_define)(dyntracer_t *dyntracer,
                                              const SEXP symbol,
                                              const SEXP value, const SEXP rho);

    /***************************************************************************
    Fires when a variable is assigned in an environment.
    ***************************************************************************/
    void (*probe_environment_variable_assign)(dyntracer_t *dyntracer,
                                              const SEXP symbol,
                                              const SEXP value, const SEXP rho);

    /***************************************************************************
    Fires when a variable is removed from an environment.
    ***************************************************************************/
    void (*probe_environment_variable_remove)(dyntracer_t *dyntracer,
                                              const SEXP symbol,
                                              const SEXP rho);

    /***************************************************************************
    Fires when a variable is looked up in an environment.
    ***************************************************************************/
    void (*probe_environment_variable_lookup)(dyntracer_t *dyntracer,
                                              const SEXP symbol,
                                              const SEXP value, const SEXP rho);

    /***************************************************************************
    Fires when a variables is checked for existence in an environment.
    ***************************************************************************/
    void (*probe_environment_variable_exists)(dyntracer_t *dyntracer,
                                              const SEXP symbol, SEXP rho);

    void *state;
};

extern int dyntrace_probe_dyntrace_entry_disabled;
extern int dyntrace_probe_dyntrace_exit_disabled;
extern int dyntrace_probe_closure_entry_disabled;
extern int dyntrace_probe_closure_exit_disabled;
extern int dyntrace_probe_builtin_entry_disabled;
extern int dyntrace_probe_builtin_exit_disabled;
extern int dyntrace_probe_special_entry_disabled;
extern int dyntrace_probe_special_exit_disabled;
extern int dyntrace_probe_promise_force_entry_disabled;
extern int dyntrace_probe_promise_force_exit_disabled;
extern int dyntrace_probe_promise_value_lookup_disabled;
extern int dyntrace_probe_promise_value_assign_disabled;
extern int dyntrace_probe_promise_expression_lookup_disabled;
extern int dyntrace_probe_promise_expression_assign_disabled;
extern int dyntrace_probe_promise_environment_lookup_disabled;
extern int dyntrace_probe_promise_environment_assign_disabled;
extern int dyntrace_probe_eval_entry_disabled;
extern int dyntrace_probe_eval_exit_disabled;
extern int dyntrace_probe_gc_entry_disabled;
extern int dyntrace_probe_gc_exit_disabled;
extern int dyntrace_probe_gc_unmark_disabled;
extern int dyntrace_probe_gc_allocate_disabled;
extern int dyntrace_probe_context_entry_disabled;
extern int dyntrace_probe_context_exit_disabled;
extern int dyntrace_probe_context_jump_disabled;
extern int dyntrace_probe_S3_generic_entry_disabled;
extern int dyntrace_probe_S3_generic_exit_disabled;
extern int dyntrace_probe_S3_dispatch_entry_disabled;
extern int dyntrace_probe_S3_dispatch_exit_disabled;
extern int dyntrace_probe_environment_variable_define_disabled;
extern int dyntrace_probe_environment_variable_assign_disabled;
extern int dyntrace_probe_environment_variable_remove_disabled;
extern int dyntrace_probe_environment_variable_lookup_disabled;
extern int dyntrace_probe_environment_variable_exists_disabled;

// ----------------------------------------------------------------------------
// STATE VARIABLES - For Internal Use Only
// ----------------------------------------------------------------------------

// the current dyntracer
extern dyntracer_t *dyntrace_active_dyntracer;
// flag to disable reentrancy check
extern int dyntrace_check_reentrancy;
// name of currently executing probe
extern const char *dyntrace_active_dyntracer_probe_name;
// state of garbage collector before the hook is triggered
extern int dyntrace_garbage_collector_state;
// flag for checking if we are in privileged mode
extern int dyntrace_privileged_mode_flag;

SEXP do_dyntrace(SEXP call, SEXP op, SEXP args, SEXP rho);
int dyntrace_is_active();
int dyntrace_should_probe();
int dyntrace_dyntracer_is_active();
int dyntrace_dyntracer_probe_is_active();
void dyntrace_disable_garbage_collector();
void dyntrace_reinstate_garbage_collector();
void dyntrace_enable_privileged_mode();
void dyntrace_disable_privileged_mode();
int dyntrace_is_priviliged_mode();
dyntracer_t *dyntracer_from_sexp(SEXP dyntracer_sexp);
SEXP dyntracer_to_sexp(dyntracer_t *dyntracer, const char *classname);
dyntracer_t *dyntracer_replace_sexp(SEXP dyntracer_sexp,
                                    dyntracer_t *new_dyntracer);
SEXP dyntracer_destroy_sexp(SEXP dyntracer_sexp,
                            void (*destroy_dyntracer)(dyntracer_t *dyntracer));
SEXP lookup_environment(SEXP rho, SEXP key);
SEXP get_promise_expression(SEXP prom);
// ----------------------------------------------------------------------------
// helpers
// ----------------------------------------------------------------------------

const char *serialize_sexp(SEXP s);
int findOp(void *addr);
int newhashpjw(const char *s);

/* Unwind the call stack in an orderly fashion */
/* calling the code installed by on.exit along the way */
/* and finally longjmping to the innermost TOPLEVEL context */
void NORET jump_to_top_ex(Rboolean, Rboolean, Rboolean, Rboolean, Rboolean);

#define dyntrace_log_error(error, ...)                                         \
    do { Rprintf("DYNTRACE LOG - ERROR - %s · %s(...) · %d - " error "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);                                            \
        dyntrace_active_dyntracer_probe_name = NULL;                           \
        jump_to_top_ex(TRUE, TRUE, TRUE, TRUE, FALSE);                         \
    } while (0);

#define dyntrace_log_warning(warning, ...)                                     \
    Rprintf("DYNTRACE LOG - WARNING - %s · %s(...) · %d - " warning "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#define dyntrace_log_info(info, ...)                                           \
    Rprintf("DYNTRACE LOG - INFO - %s · %s(...) · %d - " info "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);

#ifdef __cplusplus
}
#endif

#endif /* __DYNTRACE_H__ */
