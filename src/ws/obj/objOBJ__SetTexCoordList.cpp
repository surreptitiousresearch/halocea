/* ?SetTexCoordList@objOBJ@@QAAXHPAX@Z @0x82BCCD18 */
#include <stdint.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
// 0x82BCCD18  ?SetTexCoordList@objOBJ@@QAAXHPAX@Z
// Attach (or clear, when ptr==nullptr) the texture-coordinate list for channel `tcInd` (0..4) and
// update the shared geometry's FVF format bits. Each channel owns a group of FVF bits spaced by 5
// (one per channel): "present" (1<<36) and "compressed" (1<<41), plus two further format-bit groups
// (1<<46, 1<<51) that are cleared when the channel is removed. When attaching, the compressed bit is
// only set if the channel is flagged compressed in stateCompr (bit 0x100<<tcInd).
//
// CAVEAT: the decompiler smeared the 64-bit FVF constants across overlapping WORD/DWORD slots and
// emitted __ROR4__/extldi noise; the bit values below are decoded directly from the disasm
// (extldi/sld sequences at 0x82BCCD98+ and 0x82BCCE20+), not from the decompiler output.

void objOBJ::SetTexCoordList(unsigned int tcInd, m3dVTX *ptr)
{
    // STRONG_ASSERT: tcInd >= 0 && tcInd < 5  (obj_access.cpp:1424)
    _UnshareGeom();

    this->pGeom->pSharedGeom->texCoordList[tcInd] = ptr;

    uint64_t presentBit    = (uint64_t)1 << (36 + tcInd);
    uint64_t compressedBit = (uint64_t)1 << (41 + tcInd);
    uint64_t fmtBitB       = (uint64_t)1 << (46 + tcInd);
    uint64_t fmtBitC       = (uint64_t)1 << (51 + tcInd);
    uint64_t comprSrcBit   = (uint64_t)0x100 << tcInd; // stateCompr texcoord-compressed

    objGEOM_SHARED *pSharedGeom;
    if (ptr) {
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
        pSharedGeom = this->pGeom->pSharedGeom;
        pSharedGeom->fvf &= ~fmtBitB;
        pSharedGeom = this->pGeom->pSharedGeom;
        pSharedGeom->fvf &= ~fmtBitC;
    }
}
