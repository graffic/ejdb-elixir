#include <ejdb/ejdb.h>
#include <ejdb/bson.h>
#include <string.h>
#include <erl_nif.h>
#include "ejdb.h"
#include "utils.h"

ErlNifResourceType* COLL_RESOURCE_TYPE;

typedef struct {
    DbResource *db_resource;
    EJCOLL *coll;
} CollResource;


ERL_NIF_TERM
nif_ejdb_savebson(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    CollResource *res;
    if(!enif_get_resource(env, argv[0], COLL_RESOURCE_TYPE, (void**) &res)) {
        return enif_make_badarg(env);
    }

    ErlNifBinary bson_arg;
    if (!enif_inspect_binary(env, argv[1], &bson_arg)) {
        return enif_make_badarg(env);
    }

    bson_oid_t oid;
    ejdbsavebson3(res->coll, bson_arg.data, &oid, false);

    char buffer[25];
    bson_oid_to_string(&oid, buffer);

    return enif_make_tuple2(
        env,
        mk_atom(env, "ok"),
        char_to_binary(env, buffer)
    );
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

    ERL_NIF_TERM current = opt_list, head, tail;
    while(enif_get_list_cell(env, current, &head, &tail)) {
        current = tail;
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

static __inline__ ERL_NIF_TERM
tuple_with_coll(ErlNifEnv *env, DbResource *db, EJCOLL *coll) {
    CollResource *resource = enif_alloc_resource(
        COLL_RESOURCE_TYPE, sizeof(CollResource));
    resource->coll = coll;
    // Keep a reference to release_resource when we free the collection
    resource->db_resource = db;
    enif_keep_resource(db);

    ERL_NIF_TERM ret = enif_make_resource(env, resource);
    enif_release_resource(resource);
    return enif_make_tuple2(env, mk_atom(env, "ok"), ret);
}

#define ARG_BINARY(dest, index) \
    ErlNifBinary dest; \
    if (!enif_inspect_binary(env, argv[index], &argv_1)) {\
        return enif_make_badarg(env);\
    }

ERL_NIF_TERM
nif_ejdb_getcolls(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_badarg(env);
}

ERL_NIF_TERM
nif_ejdb_getcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ARG_DB_RESOURCE(dbr, 0)
    ARG_BINARY(argv_1, 1)

    char *coll_name = binary_to_char(&argv_1);
    EJCOLL *coll = ejdbgetcoll(dbr->db, coll_name);
    enif_free(coll_name);

    if(coll == NULL) {
        return enif_make_tuple2(
            env,
            mk_atom(env, "error"),
            char_to_binary(env, "Collection does not exist")
        );
    }

    return tuple_with_coll(env, dbr, coll);
}

ERL_NIF_TERM
nif_ejdb_createcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ARG_DB_RESOURCE(dbr, 0)
    ARG_BINARY(argv_1, 1)

    if(!enif_is_list(env, argv[2]))
    {
        return enif_make_badarg(env);
    }
    EJCOLLOPTS empty_opts = {0};
    EJCOLLOPTS *opts = parse_coll_opts(env, &empty_opts, argv[2]);

    char *coll_name = binary_to_char(&argv_1);
    EJCOLL *coll = ejdbcreatecoll(dbr->db, coll_name, opts);
    enif_free(coll_name);

    if(coll == NULL) {
        return ejdb_error_tuple(env, dbr);
    }

    return tuple_with_coll(env, dbr, coll);
}

ERL_NIF_TERM
nif_ejdb_rmcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return enif_make_badarg(env);
}

static void
free_coll_resource(ErlNifEnv *env, void *resource) {
    CollResource *cr = (CollResource *)resource;
    enif_release_resource(cr->db_resource);
}

bool
open_coll_resource(ErlNifEnv *env) {
    const char* name = "EJCOLL";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    COLL_RESOURCE_TYPE = enif_open_resource_type(
        env, NULL, name, free_coll_resource, flags, NULL);
    return (COLL_RESOURCE_TYPE != NULL);
}
