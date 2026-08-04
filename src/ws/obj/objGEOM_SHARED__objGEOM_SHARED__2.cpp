#include "objGEOM_SHARED.h"

// objGEOM_SHARED::objGEOM_SHARED(const objGEOM_SHARED &src) @ 0x82A7EE48
// Copy-construct a private clone of `src` (used by objOBJ::_UnshareGeom): memberwise-copies
// every field of the 180-byte struct.
// CAVEAT: the decompile performs this entirely as a run of overlapping QWORD/DWORD/WORD block
// copies through a pointer it calls `HIDWORD(fvfInterleaved)` — that "high dword of a 64-bit
// temp" is a decompiler mistyping: the register actually just holds `(const char *)&src` (the
// source object's own address), reused/mis-packed alongside the real fvfInterleaved value in the
// same 64-bit slot. Every one of those block copies was cross-checked against the objGEOM_SHARED
// field offsets (0x00..0xB4) and corresponds exactly to a plain field-by-field copy — reproduced
// here as ordinary member assignments (the compiler's implicit memberwise copy ctor, made
// explicit).
objGEOM_SHARED::objGEOM_SHARED(const objGEOM_SHARED &src)
{
    this->stateCompr.state = src.stateCompr.state;
    this->fvf = src.fvf;
    this->fvfInterleaved = src.fvfInterleaved;
    this->fvfInstanced = src.fvfInstanced;
    this->pSplit = src.pSplit;
    this->faceList = src.faceList;
    for (int texCoordIndex = 0; texCoordIndex < 5; ++texCoordIndex)
        this->texCoordList[texCoordIndex] = src.texCoordList[texCoordIndex];
    for (int colorIndex = 0; colorIndex < 3; ++colorIndex)
        this->colorVertList[colorIndex] = src.colorVertList[colorIndex];
    this->vertList = src.vertList;
    this->normalVertList = src.normalVertList;
    for (int tangentIndex = 0; tangentIndex < 5; ++tangentIndex)
        this->tangentList[tangentIndex] = src.tangentList[tangentIndex];
    this->vertBoneIdxFirst = src.vertBoneIdxFirst;
    this->vertBoneIdxCount = src.vertBoneIdxCount;
    this->vertBoneIdxList = src.vertBoneIdxList;
    this->vertBoneWeightList = src.vertBoneWeightList;
    this->bbox = src.bbox;
    this->pDataInst = src.pDataInst;
    this->pDataInterleaved = src.pDataInterleaved;
    for (int i = 0; i < 5; ++i) {
        this->nTexCoord.list[i] = src.nTexCoord.list[i];
        this->texCoordMaxTile.list[i] = src.texCoordMaxTile.list[i];
    }
    for (int axis = 0; axis < 3; ++axis) {
        this->vertOffset.list[axis] = src.vertOffset.list[axis];
        this->vertScale.list[axis] = src.vertScale.list[axis];
    }
    this->instNum = src.instNum;
    this->instSize = src.instSize;
    this->interleavedSize = src.interleavedSize;
}
