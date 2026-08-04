#include "../headers/hcex/HCEX_LOADING_SCREEN_DESC.h"
#include "../headers/ws/ds/ds_new.h"

// 0x823F4368 — ??R?$dsMakeFunctor@VHCEX_LOADING_SCREEN_DESC@@$0A@@@QBAPAVHCEX_LOADING_SCREEN_DESC@@PBDH@Z
// dsMakeFunctor<HCEX_LOADING_SCREEN_DESC,0>::operator()(const char *file, unsigned line):
// branded-new factory. Allocates 0x18 (24) bytes via the (size,file,line) operator new and runs
// the HCEX_LOADING_SCREEN_DESC default ctor — i.e. `new(file,line) HCEX_LOADING_SCREEN_DESC()`.
// The mangled return type is HCEX_LOADING_SCREEN_DESC* (the decompiler dropped the tail return of
// the freshly allocated pointer; disasm returns r3). Stateless functor, `this` unused.
struct dsMakeFunctor_HCEX_LOADING_SCREEN_DESC {
    HCEX_LOADING_SCREEN_DESC *operator()(const char *file, unsigned int line) const;
};

HCEX_LOADING_SCREEN_DESC *dsMakeFunctor_HCEX_LOADING_SCREEN_DESC::operator()(
    const char *file, unsigned int line) const
{
    return new (file, line) HCEX_LOADING_SCREEN_DESC();
}
