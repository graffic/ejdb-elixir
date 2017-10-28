#ifndef EJDB_H
#define EJDB_H

#include <erl_nif.h>
#include <stdbool.h>

ERL_NIF_TERM
nif_ejdb_version(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

ERL_NIF_TERM
nif_ejdb_open(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

bool
open_db_resource(ErlNifEnv *env);

#endif /* EJDB_H */
