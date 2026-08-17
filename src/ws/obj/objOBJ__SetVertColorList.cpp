/* ?SetVertColorList@objOBJ@@QAAXHPAK@Z @0x82BCB450 */
#include <stdint.h>
#include "objOBJ.h"
#include "objGEOM_UNSHARED.h"
#include "objGEOM_SHARED.h"
#include "../ds/ds_assert_boundary.h"
// 0x82BCB450  ?SetVertColorList@objOBJ@@QAAXHPAK@Z
// Install the per-vertex color array for channel `vcInd` (0..2), unsharing the geometry first,
// then flag the channel present/absent in the shared fvf.
//
// CAVEAT: the decompiler packed the pSharedGeom pointer, the 0x40000000 flag bit and the fvf
// into overlapping 64-bit slots (HIDWORD/LODWORD). Untangled against the disassembly at
// 0x82BCB4B4-0x82BCB4F0: colorVertList[vcInd] = list; presentBit = (uint64)0x40000000 << vcInd;
// fvf = list ? (fvf | presentBit) : (fvf & ~presentBit).

void objOBJ::SetVertColorList(unsigned int vcInd, unsigned int *list)
{
    if (!IGNORE_STRONG_ASSERT && vcInd > 2)
        static_cast<STRONG_ASSERT_DUMMY *>(nullptr)->Crash(
            "vcInd >= 0 && vcInd < 3",
            "D:\\Projects\\code\\common\\src.sys\\objects\\obj_access.cpp", 1392,
            empty_string);

    _UnshareGeom();

    objGEOM_SHARED *pSharedGeom = this->pGeom->pSharedGeom;
    pSharedGeom->colorVertList[vcInd] = list;

    uint64_t presentBit = (uint64_t)0x40000000 << vcInd; // channel-present flag
    if (list)
        pSharedGeom->fvf |= presentBit;
    else
        pSharedGeom->fvf &= ~presentBit;
}
