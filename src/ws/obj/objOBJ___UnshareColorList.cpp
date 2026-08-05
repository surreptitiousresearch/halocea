#include <stdint.h>
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7FBB8  ?_UnshareColorList@objOBJ@@QAAHH@Z
// If color channel `vcInd` (0..2) is shared (stateShare bit (0x100<<vcInd)), give it a private
// copy: clear the flag, drop the shared pointer, allocate a fresh list, and copy the old colors
// in. Returns 1 on success (including already-private / no-list cases), 0 on failure.
//
// CAVEAT: the decompiler fabricated phantom register args and packed the share bit / stateShare
// value into overlapping 64-bit slots; untangled against the disasm — shareBit = 0x100<<vcInd,
// colorVertList[vcInd] is the shared color array.

int objOBJ::_UnshareColorList(unsigned int vcInd)
{
    if (!this->pGeom || vcInd > 2)
        return 0;

    uint64_t shareBit = (uint64_t)0x100 << vcInd; // OBJ_ST_SHARED_COLOR<vcInd>
    if ((this->stateShare.state & shareBit) == 0)
        return 1; // already private

    _UnshareGeom();
    this->stateShare.state &= ~shareBit;

    unsigned int *oldColors = this->pGeom->pSharedGeom->colorVertList[vcInd];
    if (!oldColors)
        return 1;

    SetVertColorList(vcInd, nullptr);
    if (!_AllocColorList(vcInd, this->nVert))
        return 0;

    memcpy(this->pGeom->pSharedGeom->colorVertList[vcInd], oldColors, 4 * (size_t)this->nVert);
    return 1;
}
