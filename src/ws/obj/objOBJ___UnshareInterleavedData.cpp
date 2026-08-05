#include <stdint.h>
#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A80288  ?_UnshareInterleavedData@objOBJ@@QAAXXZ
// If this object shares its interleaved vertex buffer (stateShare bit 0x400000), give it a
// private copy: clear the flag, unshare the geometry, allocate a fresh buffer of the same
// format/size, and memcpy the shared contents into it.
//
// CAVEAT: the decompiler packed pSharedGeom / pDataInterleaved into overlapping 64-bit slots.
// Untangled against the disassembly at 0x82A802E4-0x82A8034C: SetInterleavedData is called with
// (NULL, 0, 0) to detach, then _AllocInterleavedData(nVert, interleavedSize, fvfInterleaved).

void objOBJ::_UnshareInterleavedData()
{
    if (!this->pGeom || (this->stateShare.state & 0x400000) == 0)
        return;

    _UnshareGeom();
    this->stateShare.state &= ~0x400000; // clear "interleaved data shared" flag

    objGEOM_SHARED *pSharedGeom = this->pGeom->pSharedGeom;
    if (!pSharedGeom->pDataInterleaved)
        return;

    void            *sharedData      = pSharedGeom->pDataInterleaved;
    uint8_t  interleavedSize = pSharedGeom->interleavedSize;
    uint64_t fvfInterleaved  = pSharedGeom->fvfInterleaved;

    SetInterleavedData(nullptr, 0, 0); // detach the shared buffer
    _AllocInterleavedData(this->nVert, interleavedSize, fvfInterleaved); // private copy
    memcpy(this->pGeom->pSharedGeom->pDataInterleaved, sharedData,
           (size_t)(this->nVert * interleavedSize));
}
