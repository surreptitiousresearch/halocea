/* ?_AllocVertList@objOBJ@@QAAHH@Z @0x82A7EFD0 */
#include <stdint.h>
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7EFD0  ?_AllocVertList@objOBJ@@QAAHH@Z
// (Re)allocate the vertex-position array and set the live vertex count to `nVert`. Element
// size is 12 bytes (m3dV) uncompressed, else 6 or 8 bytes depending on the compression state
// bits (stateCompr bit 0x1 = compressed, bit 0x2 selects the wider 8-byte form). New tail
// slots are zero-filled. Returns 1 on success (or when nVert==0), 0 on allocation failure.

int objOBJ::_AllocVertList(int nVert)
{
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (!pGeom)
        return 0;

    int64_t compr = pGeom->pSharedGeom->stateCompr.state;
    int elemSize = 12; // m3dV
    if (compr & 1)
        elemSize = (compr & 2) == 0 ? 6 : 8;

    char *verts = (char *)AllocObjData(0, elemSize * nVert, pGeom->pSharedGeom->vertList);
    if (!verts && nVert > 0)
        return 0;

    int oldCount = this->nVert;
    if (nVert > oldCount)
        memset(&verts[oldCount * elemSize], 0, (size_t)(nVert - oldCount) * elemSize);

    SetVertList((m3dV *)verts);
    this->nVert = nVert;
    return 1;
}
