#ifndef	_RDTRACE_H
#define	_RDTRACE_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <Defn.h>
#include <rdtrace_probes.h>

#define FUN_NO_FLAGS 0x0
#define FUN_BC 0x1

#define ALLOC_VECTOR 0x1

void rdtrace_function_entry(SEXP call, SEXP op, SEXP rho);
void rdtrace_function_exit(SEXP call, SEXP op, SEXP rho, SEXP rv);
void rdtrace_force_promise_entry(SEXP symbol);
void rdtrace_force_promise_exit(SEXP symbol, SEXP val);
void rdtrace_promise_lookup(SEXP symbol, SEXP val);
void rdtrace_builtin_entry(SEXP call);
void rdtrace_builtin_exit(SEXP call, SEXP rv);
void rdtrace_error(SEXP call, const char *message);
void rdtrace_alloc_entry(int type, SEXPTYPE sexptype, long length);
void rdtrace_alloc_exit(int type, SEXPTYPE sexptype, long length, SEXP var);

#endif	/* _RDTRACE_H */
