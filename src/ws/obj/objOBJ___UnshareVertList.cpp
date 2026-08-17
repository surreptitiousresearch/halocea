/* ?_UnshareVertList@objOBJ@@QAAHXZ @0x82A7F830 */
#include <stdint.h>
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7F830  ?_UnshareVertList@objOBJ@@QAAHXZ
// If the vertex-position list is shared (stateShare bit 0x2), give it a private copy: clear the
// flag, allocate a fresh list, and copy the old positions in (12 bytes/vertex, or 6/8 when
// compressed). Returns 1 on success (including already-private / no-list cases), 0 on failure.

int objOBJ::_UnshareVertList()
{
    if (!this->pGeom)
        return 0;

    if ((this->stateShare.state & 2) != 0) {
        _UnshareGeom();
        this->stateShare.state &= ~2ll;

        m3dV *oldVerts = this->pGeom->pSharedGeom->vertList;
        if (oldVerts) {
            SetVertList(nullptr);
            if (!_AllocVertList(this->nVert))
                return 0;

            objGEOM_UNSHARED *pGeom = this->pGeom;
            int elemSize = 12;
            if (pGeom) {
                int64_t compr = pGeom->pSharedGeom->stateCompr.state;
                if (compr & 1)
                    elemSize = (compr & 2) == 0 ? 6 : 8;
            }
            memcpy(pGeom->pSharedGeom->vertList, oldVerts, (size_t)this->nVert * elemSize);
        }
    }
    return 1;
}
