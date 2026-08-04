#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7F348  ?_AllocColorList@objOBJ@@QAAHHH@Z
// (Re)allocate the per-vertex color array for channel `vcInd` (0..2), 4 bytes/vertex. Any
// newly-exposed slots are initialised to 0xFFFFFFFF (opaque white). Returns 1 on success
// (or when nVert==0), 0 on allocation failure.

int objOBJ::_AllocColorList(unsigned int vcInd, int nVert)
{
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (!pGeom || vcInd > 2)
        return 0;

    unsigned int *oldColors = pGeom->pSharedGeom->colorVertList[vcInd];
    bool wasUnallocated = (oldColors == nullptr);

    unsigned int *colors = (unsigned int *)AllocObjData(vcInd + 9, 4 * nVert, oldColors);
    if (!colors && nVert > 0)
        return 0;

    if (wasUnallocated) {
        memset(colors, -1, 4 * nVert);
    } else {
        int oldCount = this->nVert;
        if (nVert > oldCount)
            memset(colors + oldCount, -1, 4 * (nVert - oldCount)); // fill only the new tail
    }

    SetVertColorList(vcInd, colors);
    return 1;
}
