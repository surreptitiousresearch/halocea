/* HCEX_OBJ::operator= @0x823EEB88 — copy-assign id/pInst/modelIdx, delegate the two vectors to
 * their own (already-reversed) copy-assignment operators, then copy the flags bitfield word from
 * `that`.
 *
 * Modeled as a free helper (see HCEX_OBJ__HCEX_OBJ_default.cpp for why).
 *
 * DEVIATION (flags): same unrolled-single-bit-RMW pattern as the copy ctor (see
 * HCEX_OBJ__HCEX_OBJ_copy.cpp); verified equivalent to a masked copy of bits 17..31. */

#include "../../headers/hcex/HCEX_OBJ.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/WEAK_PTR.h"
#include "../../headers/ws/m3d/m3dMATR.h"

struct entENTITY; // boundary — referenced only through the weak handle

HCEX_OBJ *hcex_obj_assign(HCEX_OBJ *obj, const HCEX_OBJ *that)
{
    obj->id = that->id;
    obj->pInst = that->pInst;
    obj->modelIdx = that->modelIdx;

    reinterpret_cast<dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> &>(obj->followers) =
        reinterpret_cast<const dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> &>(that->followers);
    reinterpret_cast<dsVECTOR<m3dMATR, 8> &>(obj->followersOfs) =
        reinterpret_cast<const dsVECTOR<m3dMATR, 8> &>(that->followersOfs);

    HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & 0x1FFFFu) | (HCEX_OBJ_FLAGS(that) & 0xFFFE0000u); // DEVIATION: see header comment
    return obj;
}
