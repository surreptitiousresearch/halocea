#include "objGEOM_SHARED.h"

extern const m3dV m3dVZero; // boundary — zero-vector constant (m3d subsystem global)

// objGEOM_SHARED::objGEOM_SHARED() @ 0x82A7E960
// Default-construct an empty shared-geometry payload: zero every count/size/flag/pointer field,
// zero the per-texcoord bookkeeping arrays, zero vertOffset and reset vertScale to identity (1),
// zero the bounding box, and start with a single (non-instanced) instance.
// CAVEAT: the decompile emits the vertOffset/vertScale writes through an inlined, assert-guarded
// ds_const_list-style bounds-checked operator[] loop (STRONG_ASSERT on "index >= 0 && index <
// Size" from ds_const_list.h:115), and stores vertOffset's zero twice (once directly, once again
// via the assert-guarded loop) — collapsed here to the equivalent direct per-element writes; the
// STRONG_ASSERT bounds check itself is not reproduced (index is a compile-time-constant 0..2,
// always in range).
objGEOM_SHARED::objGEOM_SHARED()
{
    this->stateCompr.state = 0;

    this->nTexCoord.list[0] = 0;
    this->nTexCoord.list[1] = 0;
    this->nTexCoord.list[2] = 0;
    this->nTexCoord.list[3] = 0;
    this->nTexCoord.list[4] = 0;

    this->texCoordMaxTile.list[0] = 0;
    this->texCoordMaxTile.list[1] = 0;
    this->texCoordMaxTile.list[2] = 0;
    this->texCoordMaxTile.list[3] = 0;
    this->texCoordMaxTile.list[4] = 0;

    for (int axis = 0; axis < 3; ++axis) {
        this->vertOffset.list[axis] = 0;
        this->vertScale.list[axis] = 1;
    }

    this->fvf = 0;
    this->fvfInterleaved = 0;
    this->fvfInstanced = 0;
    this->pSplit = nullptr;
    this->faceList = nullptr;
    this->texCoordList[0] = nullptr;
    this->texCoordList[1] = nullptr;
    this->texCoordList[2] = nullptr;
    this->texCoordList[3] = nullptr;
    this->texCoordList[4] = nullptr;
    this->colorVertList[0] = nullptr;
    this->colorVertList[1] = nullptr;
    this->colorVertList[2] = nullptr;
    this->vertList = nullptr;
    this->vertBoneIdxList = nullptr;
    this->vertBoneWeightList = nullptr;

    this->normalVertList = nullptr;
    this->tangentList[0] = nullptr;
    this->tangentList[1] = nullptr;
    this->tangentList[2] = nullptr;
    this->tangentList[3] = nullptr;
    this->tangentList[4] = nullptr;

    this->bbox.bll = m3dVZero;
    this->bbox.fur = m3dVZero;

    this->pDataInst = nullptr;
    this->pDataInterleaved = nullptr;
    this->instNum = 1;
    this->instSize = 0;
    this->interleavedSize = 0;
    this->vertBoneIdxFirst = 0;
    this->vertBoneIdxCount = 0;
}
