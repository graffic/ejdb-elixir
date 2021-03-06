#include "ejdb.h"
#include "ejcoll.h"

static ErlNifFunc
nif_functions[] = {
      { "version", 0, nif_ejdb_version },
      { "open", 2, nif_ejdb_open },
      { "close", 1 , nif_ejdb_close },
      { "create_collection", 3, nif_ejdb_createcoll },
      { "get_collection", 2, nif_ejdb_getcoll },
      { "get_collections", 1, nif_ejdb_getcolls },
      { "remove_collection", 3, nif_ejdb_rmcoll },
      { "save_bson", 2, nif_ejdb_savebson },
      { "load_bson", 2, nif_ejdb_loadbson }
};

static int
load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info) {
    if(!open_db_resource(env)) {
        return -1;
    }
    if(!open_coll_resource(env)) {
        return -1;
    }
    return 0;
}

static int
upgrade(ErlNifEnv* env, void** priv_data, void** old_priv_data, ERL_NIF_TERM load_info) {
    return load(env, priv_data, load_info);
}

ERL_NIF_INIT(Elixir.Ejdb, nif_functions, load, NULL, upgrade, NULL)
