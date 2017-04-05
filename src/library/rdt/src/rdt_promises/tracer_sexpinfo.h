//
// Created by nohajc on 27.3.17.
//

#ifndef R_3_3_1_TRACER_SEXPINFO_H
#define R_3_3_1_TRACER_SEXPINFO_H

#include <string>
#include <vector>
#include <functional>
#include <cassert>

#include "../r.h"

// Use generated call IDs instead of function env addresses
#define RDT_CALL_ID

#define RID_INVALID (rid_t)-1

using namespace std;
                            // Typical human-readable representation
typedef uintptr_t rid_t;    // TODO
typedef intptr_t rsid_t;    // TODO

typedef rid_t prom_addr_t;  // hexadecimal
typedef rid_t env_addr_t;   // hexadecimal
typedef rid_t fn_addr_t;    // hexadecimal
typedef rsid_t prom_id_t;   // hexadecimal
typedef rid_t call_id_t;    // integer          if RDT_CALL_ID
                            // hexadecimal      otherwise

typedef int arg_id_t;       // integer

typedef pair<fn_addr_t, string> arg_key_t;

rid_t get_sexp_address(SEXP e);

typedef tuple<string, arg_id_t, prom_id_t> arg_t;
typedef tuple<arg_id_t, prom_id_t> anon_arg_t;

class arglist_t {
    vector<arg_t> args;
    vector<arg_t> ddd_kw_args;
    vector<arg_t> ddd_pos_args;
    mutable vector<reference_wrapper<const arg_t>> arg_refs;
    mutable bool update_arg_refs;

    template<typename T>
    void push_back_tmpl(T&& value, bool ddd) {
        if (ddd) {
            arg_t new_value = value;
            string & arg_name = get<0>(new_value);
            arg_name = "...[" + arg_name + "]";
            ddd_kw_args.push_back(new_value);
        }
        else {
            args.push_back(forward<T>(value));
        }
        update_arg_refs = true;
    }

    template<typename T>
    void push_back_anon_tmpl(T&& value) {
        string arg_name = "...[" + to_string(ddd_pos_args.size()) + "]";
        // prepend string arg_name to anon_arg_t value
        arg_t new_value = tuple_cat(make_tuple(arg_name), value);

        ddd_pos_args.push_back(new_value);
        update_arg_refs = true;
    }
public:
    arglist_t() {
        update_arg_refs = true;
    }

    void push_back(const arg_t& value, bool ddd = false) {
        push_back_tmpl(value, ddd);
    }

    void push_back(arg_t&& value, bool ddd = false) {
        push_back_tmpl(value, ddd);
    }

    void push_back(const anon_arg_t& value) {
        push_back_anon_tmpl(value);
    }

    void push_back(anon_arg_t&& value) {
        push_back_anon_tmpl(value);
    }

    // Return vector of references to elements of our three inner vectors
    // so we can iterate over all of them in one for loop.
    vector<reference_wrapper<const arg_t>> all() const {
        if (update_arg_refs) {
            arg_refs.assign(args.begin(), args.end());
            arg_refs.insert(arg_refs.end(), ddd_kw_args.begin(), ddd_kw_args.end());
            arg_refs.insert(arg_refs.end(), ddd_pos_args.begin(), ddd_pos_args.end());
            update_arg_refs = false;
        }

        return arg_refs;
    }

    size_t size() const {
        if (update_arg_refs) {
            all();
        }
        return arg_refs.size();
    }
};

struct call_info_t {
    string type;
    int call_type;
    fn_addr_t fn_id;
    string fqfn; // TODO get rid of name and only leave fully qualified name (possibly just call it name...)
    string name; // For builtins
    string fn_definition;
    string loc;
    call_id_t call_id;
    env_addr_t call_ptr;
    arglist_t arguments;
};

// FIXME would it make sense to add type of action here?
struct prom_info_t {
    string name;
    prom_id_t prom_id;
    call_id_t in_call_id;
    call_id_t from_call_id;
};

prom_id_t get_promise_id(SEXP promise);
prom_id_t make_promise_id(SEXP promise, bool negative = false);
fn_addr_t get_function_id(SEXP func);
call_id_t make_funcall_id(SEXP fn_env);

// Wraper for findVar. Does not look up the value if it already is PROMSXP.
SEXP get_promise(SEXP var, SEXP rho);
arg_id_t get_argument_id(fn_addr_t function_id, const string & argument);
arglist_t get_arguments(SEXP op, SEXP rho);

#endif //R_3_3_1_TRACER_SEXPINFO_H