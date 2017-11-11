#include <erl_nif.h>
#include <string.h>
#include <ejdb/ejdb.h>
#include "ejdb.h"

/**
 * Makes an atom from existing or new
 */
ERL_NIF_TERM 
mk_atom(ErlNifEnv* env, const char* atom)
{
    ERL_NIF_TERM ret;

    if (!enif_make_existing_atom(env, atom, &ret, ERL_NIF_LATIN1))
    {
        return enif_make_atom(env, atom);
    }

    return ret;
}

/**
 * Converts a binary to char* string
 * (No encodings)
 */
char *
binary_to_char(ErlNifBinary *binary)
{
    char *str = (char *) enif_alloc(binary->size + 1);
    strncpy(str, (char *) binary->data, binary->size);
    str[binary->size] = 0;
    return str;
}

/**
 * Creates a binary (string) from a char* string.
 */
ERL_NIF_TERM
char_to_binary(ErlNifEnv* env, const char* input)
{
    int size;
    ErlNifBinary result;

    size = strlen(input);
    enif_alloc_binary(size, &result);
    memcpy(result.data, input, size);
    return enif_make_binary(env, &result);
}

/**
 * It returns a string and not an atom due to the many error levels the error 
 * code can take: ejdb and tc error codes. Too much work for now.
 */
ERL_NIF_TERM
ejdb_error(ErlNifEnv* env, DbResource *db) {
    const char *msg = ejdberrmsg(ejdbecode(db->db));
    return enif_make_tuple2(
        env,
        mk_atom(env, "error"),
        char_to_binary(env, msg)
    );
}
