/* ?_UnshareVertNormalList@objOBJ@@QAAHXZ @0x82A7FD38 */
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7FD38  ?_UnshareVertNormalList@objOBJ@@QAAHXZ
// If the vertex-normal list is shared (stateShare bit 0x8), give it a private copy: clear the
// flag, allocate a fresh list, and copy the old normals in (12 bytes/vertex, or 4 when
// compressed). Returns 1 on success (including already-private / no-list cases), 0 on failure.

int objOBJ::_UnshareVertNormalList()
{
    if (!this->pGeom)
        return 0;

    if ((this->stateShare.state & 8) != 0) {
        _UnshareGeom();
        this->stateShare.state &= ~8ll;

        m3dV *oldNormals = this->pGeom->pSharedGeom->normalVertList;
        if (oldNormals) {
            SetVertNormalList(nullptr);
            if (!_AllocVertNormalList(this->nVert))
                return 0;

            objGEOM_UNSHARED *pGeom = this->pGeom;
            int elemSize = 12;
            if (pGeom && (pGeom->pSharedGeom->stateCompr.state & 4))
                elemSize = 4;
            memcpy(pGeom->pSharedGeom->normalVertList, oldNormals, (size_t)this->nVert * elemSize);
        }
    }
    return 1;
}
