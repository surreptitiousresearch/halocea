#include <stdint.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82BCCFA8  ?SetVertTangentList@objOBJ@@QAAXHPAX@Z
// Attach (or clear, when list==nullptr) the vertex-tangent list for texcoord channel `tcInd` (0..4)
// and update the shared geometry's FVF format bits. Each channel owns a group of FVF bits spaced by
// 5 (one per channel): "present" (1<<16), "compressed" (1<<21). When attaching a list, the
// compressed bit is only set if the channel is flagged compressed in stateCompr (bit 1<<13<<tcInd).
//
// CAVEAT: the decompiler packed the FVF constants/shift into overlapping 64-bit slots; the bit
// values below are decoded from the disasm (extldi/sld sequences) — not from the decompiler output.

void objOBJ::SetVertTangentList(unsigned int tcInd, void *tangentList)
{
    // STRONG_ASSERT: tcInd >= 0 && tcInd < 5  (obj_access.cpp:1465)
    _UnshareGeom();

    objGEOM_SHARED *pSharedGeom = this->pGeom->pSharedGeom;
    pSharedGeom->tangentList[tcInd] = (m4dV *)tangentList;

    uint64_t presentBit    = (uint64_t)0x10000 << tcInd;   // FVF tangent-present
    uint64_t compressedBit = (uint64_t)0x200000 << tcInd;  // FVF tangent-compressed
    uint64_t comprSrcBit   = (uint64_t)0x2000 << tcInd;    // stateCompr tangent-compressed

    if (tangentList) {
        pSharedGeom = this->pGeom->pSharedGeom;
        pSharedGeom->fvf |= presentBit;
        if ((this->pGeom->pSharedGeom->stateCompr.state & comprSrcBit) != 0) {
            pSharedGeom = this->pGeom->pSharedGeom;
            pSharedGeom->fvf |= compressedBit;
        }
    } else {
        pSharedGeom = this->pGeom->pSharedGeom;
        pSharedGeom->fvf &= ~presentBit;
        pSharedGeom = this->pGeom->pSharedGeom;
        pSharedGeom->fvf &= ~compressedBit;
    }
}
