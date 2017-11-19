/**
 * @file utils.h
 * @brief NIF glue with C helpers
 */ 
#ifndef ELIXIR_NIF_UTILS_H
#define ELIXIR_NIF_UTILS_H

#include <erl_nif.h>

#define ATOM_MSG_TUPLE(env, atom, msg)  enif_make_tuple2(\
        env,\
        mk_atom(env, atom),\
        char_to_binary(env, msg)\
    )

#define ERROR_MSG_TUPLE(env, msg) ATOM_MSG_TUPLE(env, "error", msg)

#define NIF_FUNC_ARGS ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]

/**
 * Reuses or makes a new atom from a `char *`
 * @param env NIF Environment
 * @param atom atom name
 * @return A term with the atom.
 */ 
ERL_NIF_TERM 
mk_atom(ErlNifEnv* env, const char* atom);

/**
 * Converts a binary to a char, that you need to enif_free after use
 * @param binary The binary to extract the string from.
 * @return the `char *` with the string.
 */ 
char *
binary_to_char(ErlNifBinary *binary);

ERL_NIF_TERM
char_to_binary(ErlNifEnv* env, const char* input);

#endif /* ELIXIR_NIF_UTILS_H */
