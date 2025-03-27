#ifndef PRNGE_ENGINE_H
#define PRNGE_ENGINE_H

#include <stdint.h>

typedef struct prnge_engine32_t {
    uint32_t (*gen)(void*);
    void* state;
} prnge_engine32_t;

typedef struct prnge_engine64_t {
    uint64_t (*gen)(void*);
    void* state;
} prnge_engine64_t;

#define prnge_engine_call(engp) ((engp).gen((engp).state))

#ifndef PRNGE_VOIDCTX_EXT
#define PRNGE_VOIDCTX_EXT _void_ctx
#endif

#define PRNGE_PCAT(x, y) x ## y
#define PRNGE_CAT(x, y) PRNGE_PCAT(x, y)

#define PRNGE_VOIDCTX_DECL(fnname, rett) \
rett PRNGE_CAT(fnname, PRNGE_VOIDCTX_EXT)(void* ctx);
#define PRNGE_VOIDCTX_DEF(fnname, rett, argt) \
rett PRNGE_CAT(fnname, PRNGE_VOIDCTX_EXT)(void* ctx) \
{ return fnname((argt*)ctx); }

#endif // PRNGE_ENGINE_H