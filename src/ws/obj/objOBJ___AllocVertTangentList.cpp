#include <stdint.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82A7F508  ?_AllocVertTangentList@objOBJ@@QAAHHH@Z
// (Re)allocate the tangent-vector list for texture channel `tcInd`, sizing each element to 4
// bytes when the geometry stores that channel in packed form, else 16 bytes (full m4dV).
// Returns 1 on success (or when nVert==0), 0 on allocation failure.
//
// CAVEAT: the decompiler fabricated two trailing phantom register args (a4/a5) — the mangle
// (HH) confirms only tcInd/nVert. The compressed-format test was reconstructed from the
// disassembly: mask = (uint64)0x2000 << tcInd against pSharedGeom->stateCompr.state.

int objOBJ::_AllocVertTangentList(int tcInd, int nVert)
{
    objGEOM_UNSHARED *pGeom = this->pGeom;
    if (!pGeom)
        return 0;

    objGEOM_SHARED *pSharedGeom = pGeom->pSharedGeom;
    uint64_t comprMask = (uint64_t)0x2000 << tcInd; // packed-tangent format bit
    int elemSize = 16;                                              // full m4dV tangent
    if ((comprMask & pSharedGeom->stateCompr.state) != 0)
        elemSize = 4;                                              // packed tangent

    void *tangentData = AllocObjData(tcInd + 4, elemSize * nVert, pSharedGeom->tangentList[tcInd]);
    if (!tangentData && nVert > 0)
        return 0;

    SetVertTangentList(tcInd, tangentData);
    return 1;
}
