#ifndef ELIXIR_NIF_EJCOLL_H
#define ELIXIR_NIF_EJCOLL_H

#include <erl_nif.h>
#include <stdbool.h>

/**
 * createcoll 
 */ 
ERL_NIF_TERM
nif_ejdb_createcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);


ERL_NIF_TERM
nif_ejdb_getcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

ERL_NIF_TERM
nif_ejdb_getcolls(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

ERL_NIF_TERM
nif_ejdb_rmcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

/**
 * Save a marshalled bson into a collection
 */
ERL_NIF_TERM
nif_ejdb_savebson(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

/**
 * Sets the COLL_RESOURCE_TYPE for column resources
 */ 
bool
open_coll_resource(ErlNifEnv *env);

#endif /* ELIXIR_NIF_EJCOLL_H */
