/* ?_UnshareInstancedData@objOBJ@@QAAXXZ @0x82A80198 */
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A80198  ?_UnshareInstancedData@objOBJ@@QAAXXZ
// If the instanced data block is shared (stateShare bit 0x200000) and interleaved data is
// present, give the instanced block a private copy: clear the flag, save the old block, reset
// the instanced-data descriptor to empty, reallocate it, and copy the saved bytes back.
//
// CAVEAT: reproduced verbatim from the disasm (0x82A801A4-0x82A80278). The presence guard reads
// the INTERLEAVED slot (pDataInterleaved) while the body operates on the INSTANCED fields, and
// instNum/instSize/fvfInstanced are zeroed before the _AllocInstancedData call — so as written
// the trailing copy transfers instSize*instNum == 0 bytes. Kept faithful to the binary.

void objOBJ::_UnshareInstancedData()
{
    if (!this->pGeom)
        return;
    if ((this->stateShare.state & 0x200000) == 0)
        return;

    _UnshareGeom();
    this->stateShare.state &= ~0x200000ll; // clear OBJ_ST_SHARED_INSTANCED_DATA

    objGEOM_SHARED *pSharedGeom = this->pGeom->pSharedGeom;
    if (!pSharedGeom->pDataInterleaved)
        return;

    void *oldInstData = pSharedGeom->pDataInst;
    pSharedGeom->pDataInst = nullptr;
    pSharedGeom->instSize = 0;
    pSharedGeom->instNum = 0;
    pSharedGeom->fvfInstanced = 0;

    _AllocInstancedData(pSharedGeom->instNum, pSharedGeom->instSize, pSharedGeom->fvfInstanced);

    pSharedGeom = this->pGeom->pSharedGeom;
    memcpy(pSharedGeom->pDataInst, oldInstData,
           (size_t)pSharedGeom->instSize * pSharedGeom->instNum);
}
