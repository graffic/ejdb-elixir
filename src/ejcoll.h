#ifndef ELIXIR_NIF_EJCOLL_H
#define ELIXIR_NIF_EJCOLL_H

#include <erl_nif.h>
#include <stdbool.h>

ERL_NIF_TERM
nif_ejdb_createcoll(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]);

bool
open_coll_resource(ErlNifEnv *env);

#endif /* ELIXIR_NIF_EJCOLL_H */
