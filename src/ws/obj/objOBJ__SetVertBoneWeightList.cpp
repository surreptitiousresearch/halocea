#include <stdint.h>
#include "objOBJ.h"
// 0x82BCC900  ?SetVertBoneWeightList@objOBJ@@QAAXPAX@Z  (mangle PAX = void*; stored as float*)
// Make the geometry private, attach a per-vertex bone-weight stream, and update the shared
// geometry's flexible-vertex-format flags to advertise the weight data.
//
// CAVEAT: decompile flagged "local variable allocation has failed" — the 64-bit fvf value was
// computed in HIDWORD/LODWORD halves of an overlapped register slot. Untangled here: the FVF
// low bits carry the stream-present flags. Setting a weight list ORs in 0x100 (bone-weight
// stream present); for compressed geometry (stateCompr bit 0x20) it additionally ORs in 0x200
// (combined 0x300). Clearing the list masks off 0x100. Bit meanings inferred from the masks.

void objOBJ::SetVertBoneWeightList(float *weightList)
{
    _UnshareGeom();

    objGEOM_SHARED *sharedGeom = pGeom->pSharedGeom;
    uint64_t fvf = sharedGeom->fvf;
    sharedGeom->vertBoneWeightList = weightList;

    if (weightList) {
        fvf |= 0x100; // bone-weight stream present
        sharedGeom->fvf = fvf;
        if (sharedGeom->stateCompr.state & 0x20) { // compressed geometry
            fvf |= 0x300;
            sharedGeom->fvf = fvf;
        }
    } else {
        fvf &= ~0x100ull; // clear bone-weight stream flag
        sharedGeom->fvf = fvf;
    }
}
