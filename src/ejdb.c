#include <string.h>
#include <erl_nif.h>
#include <ejdb/ejdb.h>
#include "utils.h"

ErlNifResourceType* DB_RESOURCE_TYPE;

typedef struct {
    EJDB *db;
} DbResource;

ERL_NIF_TERM
nif_ejdb_version(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return char_to_binary(env, ejdbversion());
}

ERL_NIF_TERM
nif_ejdb_open(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    unsigned int mode;
    char *db_name;
    bool did_open;
    ERL_NIF_TERM ret;
    ErlNifBinary argv_0;
    EJDB *db;
    DbResource *resource;

    if (!enif_inspect_binary(env, argv[0], &argv_0)) {
        return enif_make_badarg(env);
    }
    db_name = binary_to_char(&argv_0);

    if(!enif_get_uint(env, argv[1], &mode))
    {
        return enif_make_badarg(env);
    }

    db = ejdbnew();
    if (db == NULL) {
        // Return tuple { :error, "message"}
        return enif_make_tuple2(
            env,
            mk_atom(env, "error"),
            char_to_binary(env, "ejdbnew failed")
        );
    }
    did_open = ejdbopen(db, db_name, mode);
    enif_free(db_name);
    if (!did_open) {
        // Return tuple { :error, "message"}
        return enif_make_tuple2(
            env,
            mk_atom(env, "error"),
            char_to_binary(env, "ejdbopen failed")
        );
    }
    resource = enif_alloc_resource(DB_RESOURCE_TYPE, sizeof(DbResource));
    resource->db = db;

    ret = enif_make_resource(env, resource);
    enif_release_resource(resource);
    // Return tuple { :ok, connection }
    return enif_make_tuple2(env, mk_atom(env, "ok"), ret);
}

static void
free_db_resource(ErlNifEnv *env, void *resource) {
    DbResource *dbr = (DbResource *)resource;
    ejdbclose(dbr->db);
    ejdbdel(dbr->db);
}

bool
open_db_resource(ErlNifEnv *env) {
    const char* name = "DbConnection";
    int flags = ERL_NIF_RT_CREATE | ERL_NIF_RT_TAKEOVER;

    DB_RESOURCE_TYPE = enif_open_resource_type(
        env, NULL, name, free_db_resource, flags, NULL);
    return (DB_RESOURCE_TYPE != NULL);
}
