#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7FEE8  ?_UnshareVertTangentList@objOBJ@@QAAHH@Z
// If tangent channel `tcInd` is shared (stateShare bit (0x10000<<tcInd)), give it a private
// copy: clear the flag, allocate a fresh list, and copy the old tangents in (16 bytes/vertex,
// or 4 when packed via stateCompr bit (0x2000<<tcInd)). Returns 1 on success (including
// already-private / no-list cases), 0 on failure.
//
// CAVEAT: the decompiler fabricated phantom register args and packed tcInd/shareBit into the
// high dword of a shared 64-bit slot; untangled against the disasm — shareBit = 0x10000<<tcInd,
// tangentList[tcInd] is the shared tangent array (offset 4*(tcInd+20)).

int objOBJ::_UnshareVertTangentList(int tcInd)
{
    if (!this->pGeom)
        return 0;

    unsigned __int64 shareBit = (unsigned __int64)0x10000 << tcInd; // OBJ_ST_SHARED_TANGENT<tcInd>
    if ((this->stateShare.state & shareBit) != 0) {
        _UnshareGeom();
        this->stateShare.state &= ~shareBit;

        void *oldTangents = this->pGeom->pSharedGeom->tangentList[tcInd];
        if (oldTangents) {
            SetVertTangentList(tcInd, nullptr);
            if (!_AllocVertTangentList(tcInd, this->nVert))
                return 0;

            objGEOM_UNSHARED *pGeom = this->pGeom;
            int elemSize = 16; // full m4dV tangent
            if (pGeom &&
                (pGeom->pSharedGeom->stateCompr.state & ((unsigned __int64)0x2000 << tcInd)))
                elemSize = 4; // packed tangent
            memcpy(pGeom->pSharedGeom->tangentList[tcInd], oldTangents,
                   (size_t)this->nVert * elemSize);
        }
    }
    return 1;
}
