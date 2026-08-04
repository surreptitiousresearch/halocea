#include "../headers/hcex/HCEX_LOADING_SCREEN.h"
#include "../headers/ws/ds/ds_new.h"

// 0x823F3F90 — ??R?$dsMakeFunctor@VHCEX_LOADING_SCREEN@@$0A@@@QBAPAVHCEX_LOADING_SCREEN@@PBDH@Z
// dsMakeFunctor<HCEX_LOADING_SCREEN,0>::operator()(const char *file, unsigned line): branded-new
// factory. Allocates 0x58 (88) bytes via the (size,file,line) operator new, base-constructs
// ui_new::UI_FRAME, installs HCEX_LOADING_SCREEN's vtable, zeroes loadingScreenProgress and
// creates the shared D3D resources — i.e. exactly `new(file,line) HCEX_LOADING_SCREEN()`. The
// functor is stateless, so `this` is unused.
struct dsMakeFunctor_HCEX_LOADING_SCREEN {
    HCEX_LOADING_SCREEN *operator()(const char *file, unsigned int line) const;
};

HCEX_LOADING_SCREEN *dsMakeFunctor_HCEX_LOADING_SCREEN::operator()(
    const char *file, unsigned int line) const
{
    return new (file, line) HCEX_LOADING_SCREEN();
}
