/**
 * @file ni_args.h
 * @brief Helpers to handle arguments
 * 
 * These macros have implicit requirements
 * - the NIF environment exists in the env variabble
 * - the argument list is in the argv variable
 * - You need to have colls or db includes to get their resources
 */ 
#ifndef ELIXIR_NIF_NIF_ARGS_H
#define ELIXIR_NIF_NIF_ARGS_H

#define ARG_LIST(index) \
    if(!enif_is_list(env, argv[index])) {\
        return enif_make_badarg(env);\
    }

#define ARG_BINARY(dest, index) \
    ErlNifBinary dest;\
    if (!enif_inspect_binary(env, argv[index], &dest)) {\
        return enif_make_badarg(env);\
    }

#define ARG_DB_RESOURCE(db, index) \
    DbResource *db;\
    if(!enif_get_resource(env, argv[index], DB_RESOURCE_TYPE, (void**) &db)) {\
        return enif_make_badarg(env);\
    }

#define ARG_COLL_RESOURCE(coll, index) \
    CollResource *coll;\
    if(!enif_get_resource(env, argv[index], COLL_RESOURCE_TYPE, (void**) &coll)) {\
        return enif_make_badarg(env);\
    }

// #define ARG_STRING(dest, index) \
//     ErlNifBinary P99_LINEID(binary); \
//     if (!enif_inspect_binary(env, argv[index], &P99_LINEID(binary))) {\
//         return enif_make_badarg(env);\
//     }\
//     char *dest = binary_to_char(P99_LINEID(binary))

#endif
