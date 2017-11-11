#ifndef ELIXIR_NIF_UTILS_H
#define ELIXIR_NIF_UTILS_H

#include <erl_nif.h>

ERL_NIF_TERM 
mk_atom(ErlNifEnv* env, const char* atom);

char *
binary_to_char(ErlNifBinary *binary);

ERL_NIF_TERM
char_to_binary(ErlNifEnv* env, const char* input);

#define ERROR_MSG_TUPLE(env, msg) enif_make_tuple2(\
        env,\
        mk_atom(env, "error"),\
        char_to_binary(env, msg)\
    );

#endif /* ELIXIR_NIF_UTILS_H */
