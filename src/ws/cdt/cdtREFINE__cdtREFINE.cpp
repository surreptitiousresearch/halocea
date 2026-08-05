// cdtREFINE::cdtREFINE @0x827139E8 — construct a collision-query refinement filter.
// DEVIATION: the DB/decompiler ABI passes a single packed __int64 `stateObjUsrIncl` whose low
// dword is the user object-inclusion state and whose high dword becomes `layerMask` (HIDWORD).
// The nominal 4-arg source signature (flags, layerMask, apSTATE_T<__int64>, animINST*) is
// realized at runtime as this 3-register form.
#include <stdint.h>
#include "../../headers/ws/cdt/cdtREFINE.h"

// &cdtREFINE::`vftable' — emitted vtable symbol. boundary.
extern cdtREFINE_vtbl cdtREFINE__vftable;

cdtREFINE::cdtREFINE(unsigned int flags, int64_t stateObjUsrIncl, animINST *pInstSelf)
{
    weakSelf.pHandle = nullptr;
    __vftable = &cdtREFINE__vftable;
    this->stateObjUsrIncl.state = stateObjUsrIncl;
    this->pInstSelf = pInstSelf;
    state.state = flags;
    layerMask = (unsigned int)(stateObjUsrIncl >> 32); // HIDWORD(stateObjUsrIncl)
    specMode = (SELDOM)5; // raw ctor writes 5, outside the named SELDOM range (0..3)
}
