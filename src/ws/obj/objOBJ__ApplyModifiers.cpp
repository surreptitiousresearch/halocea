#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objMODIFIER.h"
// 0x82A8AE30  ?ApplyModifiers@objOBJ@@QAAHH@Z
// Run each enabled modifier whose valid-state mask overlaps `stateVldMask`. Returns 1 if every
// applicable modifier accepted, 0 as soon as one vetoes (its Modify hook returns 0).

int objOBJ::ApplyModifiers(int stateVldMask)
{
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (!pGeom->nModifiers)
        return 1;
    if (pGeom->nModifiers <= 0)
        return 1;

    int appliedCount = 0;
    for (int i = 0; ; ++i) {
        objMODIFIER *mod = pGeom->modList[i];
        if ((mod->stateVld.state & stateVldMask) != 0 &&
            (mod->state.state & 1) == 0 &&
            !mod->__vftable->Modify(mod))
            break; // this modifier vetoed the update

        pGeom = this->pGeom;
        if (++appliedCount >= pGeom->nModifiers)
            return 1;
    }
    return 0;
}
