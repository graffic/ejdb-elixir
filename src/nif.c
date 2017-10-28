#include "ejdb.h"

static ErlNifFunc
nif_functions[] = {
      { "version", 0, nif_ejdb_version },
      { "open", 2, nif_ejdb_open }
};

static int
load(ErlNifEnv* env, void** priv_data, ERL_NIF_TERM load_info) {
    if(!open_db_resource(env)) {
        return -1;
    }
    return 0;
}

ERL_NIF_INIT(Elixir.Ejdb, nif_functions, load, NULL, NULL, NULL)
