/* ?SetToLocalVarRef@sslSTACK_DATA@@QAAAAV1@PAVsslSTACK@@H@Z @0x82AF08C0 */
#include "sslSTACK_DATA.h"

// 0x82AF08C0 — sslSTACK_DATA::SetToLocalVarRef. Stores a (stack, index) local-variable reference
// into the localVar arm.
sslSTACK_DATA &sslSTACK_DATA::SetToLocalVarRef(sslSTACK *pStack, int idx)
{
    Cleanup();
    localVar.pStack = pStack;
    localVar.idx = idx;
    type = TYPE_SSLFUNC_LOCAL_VAR_REF;
    return *this;
}
