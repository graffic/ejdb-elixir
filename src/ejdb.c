#include <string.h>
#include <erl_nif.h>
#include <ejdb/ejdb.h>

static ERL_NIF_TERM
version(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) {
    ErlNifBinary result;
    const char *output;
    int size;

    output = ejdbversion();
    size = strlen(output);
    enif_alloc_binary(size, &result);
    memcpy(result.data, output, size);
    return enif_make_binary(env, &result);
}

static ErlNifFunc nif_functions[] = {
      { "version", 0, version }
};

ERL_NIF_INIT(Elixir.Ejdb, nif_functions, NULL, NULL, NULL, NULL)
