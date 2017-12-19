#ifndef ELIXIR_NIF_EJCOLL_H
#define ELIXIR_NIF_EJCOLL_H

#include <erl_nif.h>
#include <stdbool.h>
#include "utils.h"

/**
 * createcoll 
 */ 
ERL_NIF_TERM
nif_ejdb_createcoll(NIF_FUNC_ARGS);


ERL_NIF_TERM
nif_ejdb_getcoll(NIF_FUNC_ARGS);

ERL_NIF_TERM
nif_ejdb_getcolls(NIF_FUNC_ARGS);

ERL_NIF_TERM
nif_ejdb_rmcoll(NIF_FUNC_ARGS);

/**
 * Save a marshalled bson into a collection
 */
ERL_NIF_TERM
nif_ejdb_savebson(NIF_FUNC_ARGS);

/**
 * Load a bson by `oid` in char* format
 */
ERL_NIF_TERM
nif_ejdb_loadbson(NIF_FUNC_ARGS);

/**
 * Sets the COLL_RESOURCE_TYPE for column resources
 */ 
bool
open_coll_resource(ErlNifEnv *env);

#endif /* ELIXIR_NIF_EJCOLL_H */
