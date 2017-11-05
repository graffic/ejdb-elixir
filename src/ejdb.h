#ifndef ELIXIR_NIF_EJDB_H
#define ELIXIR_NIF_EJDB_H

#include <erl_nif.h>
#include <stdbool.h>
#include <ejdb/ejdb.h>

typedef struct {
    EJDB *db;
} DbResource;

extern ErlNifResourceType* DB_RESOURCE_TYPE;

ERL_NIF_TERM
nif_ejdb_version(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

ERL_NIF_TERM
nif_ejdb_open(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

bool
open_db_resource(ErlNifEnv *env);

#endif /* ELIXIR_NIF_EJDB_H */
