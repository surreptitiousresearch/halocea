#include "objOBJ.h"
#include "obj_boundary.h"
// 0x82A7E560  ?_FreeModifierList@objOBJ@@QAAXXZ
// Release the per-object modifier list held by the unshared geometry.

void objOBJ::_FreeModifierList()
{
    objGEOM_UNSHARED *geom = pGeom;
    if (geom)
    {
        dlFree(geom->modList);
        pGeom->modList = nullptr;
    }
}
