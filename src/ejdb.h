#ifndef ELIXIR_NIF_EJDB_H
#define ELIXIR_NIF_EJDB_H

#include <erl_nif.h>
#include <stdbool.h>
#include <ejdb/ejdb.h>
#include "utils.h"

typedef struct {
    EJDB *db;
} DbResource;

extern ErlNifResourceType* DB_RESOURCE_TYPE;

ERL_NIF_TERM
nif_ejdb_version(NIF_FUNC_ARGS);

ERL_NIF_TERM
nif_ejdb_open(NIF_FUNC_ARGS);

ERL_NIF_TERM
nif_ejdb_close(NIF_FUNC_ARGS);

/**
 * Makes a tuple of {:error, "EJDB description of the error"}
 * 
 * @param env current NIF environment.
 * @param db the ejdb nif resource.
 * @return tuple2 with {:error, "message"}
 */
ERL_NIF_TERM
ejdb_error_tuple(ErlNifEnv* env, DbResource *db);

bool
open_db_resource(ErlNifEnv *env);

#endif /* ELIXIR_NIF_EJDB_H */
