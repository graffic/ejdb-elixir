#include <ejdb/ejdb.h>
#include <erl_nif.h>
#include <string.h>
#include "ejdb.h"
#include "utils.h"

ErlNifResourceType *DB_RESOURCE_TYPE;

ERL_NIF_TERM
nif_ejdb_version(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    return char_to_binary(env, ejdbversion());
}

static char *open_options[] = {
    "jboreader",    /**< 0 Open as a reader. */
    "jbowriter",     /**< 1 Open as a writer. */
    "jbocreat",     /**< 2 Create if db file not exists. */
    "jbotrunc",     /**< 3 Truncate db on open. */
    "jbonolck",     /**< 4 Open without locking. */
    "jbolcknb",     /**< 5 Lock without blocking. */
    "jbotsync"      /**< 6 Synchronize every transaction. */
};

/**
 * Parses a list of atoms with ejdb open modes
 * @param env NIF environment
 * @param opt_list List with atoms.
 * @return The final open mode
 */
static unsigned int
parse_db_opts(ErlNifEnv *env, const ERL_NIF_TERM opt_list) {
    unsigned int mode = 0;
    ERL_NIF_TERM current = opt_list, head, tail;
    while(enif_get_list_cell(env, current, &head, &tail)) {
        current = tail;

        char option[10];
        if(!enif_get_atom(env, head, option, 10, ERL_NIF_LATIN1)) {
            continue;
        }

        for(int i=0; i < 7; i++) {
            if(strcmp(option, open_options[i]) == 0) {
                mode |= 1 << i;
                break;
            }
        }
    }
    return mode;
}

/**
 * It returns a string and not an atom due to the many error levels the error 
 * code can take: ejdb and tc error codes. Too much work for now.
 */
ERL_NIF_TERM
ejdb_error_tuple(ErlNifEnv* env, DbResource *db) {
    const char *msg = ejdberrmsg(ejdbecode(db->db));
    return ERROR_MSG_TUPLE(env, msg);
}

ERL_NIF_TERM
nif_ejdb_open(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary argv_0;
    if (!enif_inspect_binary(env, argv[0], &argv_0)) {
        return enif_make_badarg(env);
    }

    if(!enif_is_list(env, argv[1]))
    {
        return enif_make_badarg(env);
    }
    unsigned int mode = parse_db_opts(env, argv[1]);

    EJDB *db = ejdbnew();
    if(db == NULL) {
        return ERROR_MSG_TUPLE(env, "error in ejdbnew"); 
    }

    char *db_name = binary_to_char(&argv_0);
    bool did_open = ejdbopen(db, db_name, mode);
    enif_free(db_name);

    if (!did_open) {
        // Return tuple { :error, "message"}
        DbResource tmp = {db};
        return ejdb_error_tuple(env, &tmp);
    }
    DbResource *resource = enif_alloc_resource(DB_RESOURCE_TYPE, sizeof(DbResource));
    resource->db = db;

    ERL_NIF_TERM ret = enif_make_resource(env, resource);
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
