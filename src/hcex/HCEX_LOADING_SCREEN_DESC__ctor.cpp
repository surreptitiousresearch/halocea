#include <new>
/* HCEX_LOADING_SCREEN_DESC::HCEX_LOADING_SCREEN_DESC @0x823F40D8 — chain to dscDESC's ctor
 * (which transiently sets __vftable to sslDESC's, per the sslDESC/dscDESC construction-order
 * idiom), zero isUnshared, construct sslClass, zero isSslClassInited, then set this class's own
 * vtable last. */

#include "../headers/hcex/HCEX_LOADING_SCREEN_DESC.h"

extern void *sslDESC_vftable; // sslDESC::`vftable' — the transitional vtable dscDESC's ctor installs

/* CAVEAT: dscDESC.h/sslDESC.h declare no constructors of their own (dscDESC::dscDESC and
 * dsREF_COUNT's ctor are boundary, not reversed elsewhere in this corpus), so the implicit
 * default-init below approximates `dscDESC::dscDESC(this)` rather than reproducing it -- it
 * zero-initializes the base subobjects instead of running dscDESC's real ctor body. */

HCEX_LOADING_SCREEN_DESC::HCEX_LOADING_SCREEN_DESC() : ui_new::UI_FRAME_DESC()
{
    isUnshared = false;
    this->__vftable = (dscDESC_vtbl *)&sslDESC_vftable;
    new (&sslClass) sslCLASS_REF();
    isSslClassInited.val = false;
    this->__vftable = (dscDESC_vtbl *)&HCEX_LOADING_SCREEN_DESC::vftable;
}
