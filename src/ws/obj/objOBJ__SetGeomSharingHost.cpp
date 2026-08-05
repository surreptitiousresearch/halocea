#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "../ds/ds_assert_boundary.h"
// 0x82BCB288  ?SetGeomSharingHost@objOBJ@@QAAXPAV1@@Z
// Record `pObj` as the object this one borrows its geometry from. Asserts the geometry exists
// and is not already hosted by a different object. Flags the host (bit 0x10 in its state).

void objOBJ::SetGeomSharingHost(objOBJ *pObj)
{
    if (!IGNORE_STRONG_ASSERT) {
        objGEOM_UNSHARED *pGeom = this->pGeom;
        objOBJ *host;
        if (!pGeom || ((host = pGeom->pSharingHostObj) != nullptr && host != pObj && pObj))
            STRONG_ASSERT_DUMMY().Crash(
                "IsGeom() && (pGeom->pSharingHostObj == 0 || pGeom->pSharingHostObj == pObj || pObj == 0)",
                "D:\\Projects\\code\\common\\src.sys\\objects\\obj_access.cpp", 60,
                empty_string);
    }

    this->pGeom->pSharingHostObj = pObj;
    if (pObj)
        pObj->state |= 0x10u;
}
