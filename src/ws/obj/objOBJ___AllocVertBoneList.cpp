#include <string.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "obj_boundary.h"
// 0x82A7F0F0  ?_AllocVertBoneList@objOBJ@@QAAXH@Z
// (Re)allocate the skin weight and index lists for `nVertNew` vertices. Per-vertex element counts are
// weight=4 (1 compressed, stateCompr bit 0x20) and index=2 (1 compressed, stateCompr bit 0x10), each
// 4 bytes wide. The weight list is skipped when state bit 8 is set. Clears the two skin-shared flags.
//
// CAVEAT: the newly-grown tail memsets use a byte offset of 4*oldVert*stride but a byte count of only
// (nVertNew-oldVert)*stride (missing the 4x element width) — reproduced verbatim from the disasm at
// 0x82A7F2E8/0x82A7F308.

void objOBJ::_AllocVertBoneList(int nVertNew)
{
    // STRONG_ASSERT: IsGeom() (Obj_alloc.cpp:372);
    // STRONG_ASSERT: !stateShare.Is(OBJ_ST_SHARED_SKIN_WEIGHTS | OBJ_ST_SHARED_SKIN_INDICES) (374)
    int weightStride = 4;
    this->stateShare.state &= ~0xC0u; // clear OBJ_ST_SHARED_SKIN_WEIGHTS|OBJ_ST_SHARED_SKIN_INDICES

    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (pGeom && (pGeom->pSharedGeom->stateCompr.state & 0x20) != 0)
        weightStride = 1;
    int idxStride = 2;
    if (pGeom && (pGeom->pSharedGeom->stateCompr.state & 0x10) != 0)
        idxStride = 1;

    char *weightList = nullptr;
    if ((this->state & 8) == 0) { // state bit 8: skip the per-vertex weight list
        void *oldWeights = GetVertBoneWeightList(0);
        weightList = (char *)(objOBJ::dataAlloc
            ? objOBJ::dataAlloc(this, 1, 4 * weightStride * nVertNew, oldWeights)
            : dlRealloc(oldWeights, 4 * weightStride * nVertNew,
                        "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0x50D));
    }

    void *oldIndices = GetVertBoneIdxList(0);
    char *idxList = (char *)(objOBJ::dataAlloc
        ? objOBJ::dataAlloc(this, 2, 4 * idxStride * nVertNew, oldIndices)
        : dlRealloc(oldIndices, 4 * idxStride * nVertNew,
                    "D:\\Projects\\code\\common\\src.sys\\objects\\Obj_alloc.cpp", 0x50D));

    int oldVertCount = this->nVert;
    if (nVertNew > oldVertCount) {
        if (weightList)
            memset(&weightList[4 * oldVertCount * weightStride], 0, (nVertNew - oldVertCount) * weightStride);
        memset(&idxList[4 * this->nVert * idxStride], 0, (nVertNew - this->nVert) * idxStride);
    }

    SetVertBoneWeightList((float *)weightList);
    SetVertBoneIdxList((unsigned __int8 *)idxList);
}
