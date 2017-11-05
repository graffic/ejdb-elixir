#include <ejdb/ejdb.h>
#include <string.h>
#include <erl_nif.h>
#include "ejdb.h"
#include "utils.h"

ErlNifResourceType* COLL_RESOURCE_TYPE;

typedef struct {
    EJCOLL *coll;
} CollResource;

ERL_NIF_TERM
nif_ejdb_getcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_badarg(env);
}

ERL_NIF_TERM
nif_ejdb_getcolls(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_badarg(env);
}

/**
 * Parses a list of tuple with pairs: option name: value
 * - :large bool
 * - :compressed bool
 * - :records integer
 * - :cachedrecords integer
 */
static EJCOLLOPTS *
parse_coll_opts(ErlNifEnv *env, EJCOLLOPTS *opts, const ERL_NIF_TERM opt_list) {
    if(enif_is_empty_list(env, opt_list)) {
        return NULL;
    }

    ERL_NIF_TERM head, tail;
    while(enif_get_list_cell(env, opt_list, &head, &tail)) {
        int arity;
        const ERL_NIF_TERM *items;
        if(!enif_get_tuple(env, head, &arity, &items) || arity != 2) {
            continue;
        }
        char option[14];
        if(!enif_get_atom(env, items[0], option, 14, ERL_NIF_LATIN1)) {
            continue;
        }

        long records;
        int cachedrecords;
        if(strcmp(option, "large") == 0 && 
           enif_get_atom(env, items[1], option, 14, ERL_NIF_LATIN1)) {
            opts->large = strcmp(option, "true") == 0;
        }
        else if(strcmp(option, "compressed") == 0 && 
                enif_get_atom(env, items[1], option, 14, ERL_NIF_LATIN1)) {
            opts->compressed = strcmp(option, "true") == 0;
        }
        else if(strcmp(option, "records") == 0 && 
                enif_get_int64(env, items[1], &records)) {
            opts->records = records;
        }
        else if(strcmp(option, "cachedrecords") == 0 && 
                enif_get_int(env, items[1], &cachedrecords)) {
            opts->cachedrecords = cachedrecords;
        }
    }
    return opts;
}

ERL_NIF_TERM
nif_ejdb_createcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    DbResource *db;
    if(!enif_get_resource(env, argv[0], DB_RESOURCE_TYPE, (void**) &db)) {
        return enif_make_badarg(env);
    }

    ErlNifBinary argv_1;
    if (!enif_inspect_binary(env, argv[1], &argv_1)) {
        return enif_make_badarg(env);
    }

    if(!enif_is_list(env, argv[2]))
    {
        return enif_make_badarg(env);
    }
    EJCOLLOPTS empty_opts = {0};
    EJCOLLOPTS *opts = parse_coll_opts(env, &empty_opts, argv[2]);

    char *coll_name = binary_to_char(&argv_1);
    EJCOLL *coll = ejdbcreatecoll(db->db, coll_name, opts);
    enif_free(coll_name);

    if(coll == NULL) {
        return ejdb_error(env, db);
    }

    CollResource *resource = enif_alloc_resource(
        COLL_RESOURCE_TYPE, sizeof(CollResource));
    resource->coll = coll;
    ERL_NIF_TERM ret = enif_make_resource(env, resource);
    enif_release_resource(resource);
    return enif_make_tuple2(env, mk_atom(env, "ok"), ret);
}

ERL_NIF_TERM
nif_ejdb_rmcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_badarg(env);
}

bool
open_coll_resource(ErlNifEnv *env) {
    const char* name = "EJCOLL";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    COLL_RESOURCE_TYPE = enif_open_resource_type(
        env, NULL, name, NULL, flags, NULL);
    return (COLL_RESOURCE_TYPE != NULL);
}
