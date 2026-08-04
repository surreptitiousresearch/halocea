#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A81C40  ?_UnshareTexCoord@objOBJ@@QAAHH@Z
// If texcoord channel `tcInd` (0..4) is shared (stateShare bit 0x800<<tcInd), give it a private
// copy: clear the flag, drop the shared list, reallocate, and copy the old contents in. Element
// stride is 8 bytes, or 4 when the shared geometry flags the channel compressed (stateCompr bit
// 0x100<<tcInd). Returns 1 on success (incl. already-private / no-list), 0 on allocation failure.
//
// CAVEAT: the decompiler fabricated phantom register args and packed the share bit / channel index
// into overlapping 64-bit slots; untangled against the disasm.

int objOBJ::_UnshareTexCoord(unsigned int tcInd)
{
    if (!this->pGeom || tcInd > 4)
        return 0;

    unsigned __int64 shareBit = (unsigned __int64)0x800 << tcInd; // OBJ_ST_SHARED_TEXCOORD<tcInd>
    if ((this->stateShare.state & shareBit) == 0)
        return 1; // already private

    _UnshareGeom();
    this->stateShare.state &= ~shareBit;

    if (!GetTexCoordList(tcInd))
        return 1;

    const void *oldList = GetTexCoordList(tcInd);
    SetTexCoordList(tcInd, nullptr);
    if (!_AllocTexCoord(tcInd, this->nVert))
        return 0;

    int stride = 8;
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (pGeom && (pGeom->pSharedGeom->stateCompr.state & ((unsigned __int64)0x100 << tcInd)) != 0)
        stride = 4;

    memcpy(GetTexCoordList(tcInd), oldList, (size_t)this->nVert * stride);
    return 1;
}
