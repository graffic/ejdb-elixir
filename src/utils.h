#ifndef UTILS_H
#define UTILS_H

ERL_NIF_TERM 
mk_atom(ErlNifEnv* env, const char* atom);

char *
binary_to_char(ErlNifBinary *binary);

ERL_NIF_TERM
char_to_binary(ErlNifEnv* env, const char* input);

#endif /* UTILS_H */
