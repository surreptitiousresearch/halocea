#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "obj_boundary.h"
// 0x82A7F960  ?_UnshareVertBoneList@objOBJ@@QAAXXZ
// If the skin index and/or weight lists are shared (stateShare bits 0x40 weights / 0x80 indices),
// give them private copies: clear the shared flags, stash the shared buffers, reallocate fresh ones,
// copy the data back, and free the stashed buffers we owned. Index stride is 8 bytes (4 compressed,
// stateCompr bit 0x10); weight stride is 16 bytes (4 compressed, stateCompr bit 0x20).
//
// CAVEAT: the decompiler expressed the "was-not-shared" tests via _cntlzw bit tricks; decoded to the
// plain (state & bit) == 0 predicates below.

void objOBJ::_UnshareVertBoneList()
{
    if (!this->pGeom || (this->stateShare.state & 0xC0) == 0)
        return;

    _UnshareGeom();
    __int64 state = this->stateShare.state;
    bool ownIndices = (state & 0x80) == 0; // OBJ_ST_SHARED_SKIN_INDICES not set -> we own the buffer
    bool ownWeights = (state & 0x40) == 0; // OBJ_ST_SHARED_SKIN_WEIGHTS not set -> we own the buffer
    this->stateShare.state &= ~0xC0u;  // clear the two skin-shared flags

    objGEOM_UNSHARED *pGeom = this->pGeom;
    int idxStride = 8;
    if (pGeom && (pGeom->pSharedGeom->stateCompr.state & 0x10) != 0)
        idxStride = 4;
    int weightStride = 16;
    if (pGeom && (pGeom->pSharedGeom->stateCompr.state & 0x20) != 0)
        weightStride = 4;

    void *savedIdx = nullptr;
    if (GetVertBoneIdxList(0)) {
        savedIdx = GetVertBoneIdxList(0);
        SetVertBoneIdxList(nullptr);
    }
    void *savedWeight = nullptr;
    if (GetVertBoneWeightList(0)) {
        savedWeight = GetVertBoneWeightList(0);
        SetVertBoneWeightList(nullptr);
    }

    _AllocVertBoneList(this->nVert);

    if (savedIdx) {
        memcpy(GetVertBoneIdxList(0), savedIdx, (size_t)this->nVert * idxStride);
        if (ownIndices) {
            if (objOBJ::dataFree)
                objOBJ::dataFree(this, savedIdx, 2);
            else
                dlFree(savedIdx);
        }
    }
    if (savedWeight) {
        memcpy(GetVertBoneWeightList(0), savedWeight, (size_t)this->nVert * weightStride);
        if (ownWeights) {
            if (objOBJ::dataFree)
                objOBJ::dataFree(this, savedWeight, 1);
            else
                dlFree(savedWeight);
        }
    }
}
