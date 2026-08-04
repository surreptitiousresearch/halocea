#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objMODIFIER.h"
// 0x82A8AD28  ?RemoveModifiers@objOBJ@@QAAXXZ
// Detach and destroy every geometry modifier attached to this object, then drop the list.
// Each modifier's DelObj hook runs first, followed by its scalar-deleting destructor.

void objOBJ::RemoveModifiers()
{
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (!pGeom)
        return;

    for (; pGeom->nModifiers > 0; pGeom = this->pGeom) {
        objMODIFIER *mod = *pGeom->modList; // always peel off the first live modifier
        mod->__vftable->DelObj(mod);
        // Scalar-deleting dtr: deleteFlag=1 => run ~objMODIFIER then free (vtbl slot models the flag).
        mod->__vftable->dtr_objMODIFIER(mod, 1);
    }

    _FreeModifierList();
    this->pGeom->nModifiers = 0;
}
