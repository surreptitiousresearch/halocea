/* HCEX_OBJ copy ctor @0x823EECE0 — copy id/pInst/modelIdx verbatim, adopt `that`'s two vectors'
 * call-site cookies and Insert-copy their full ranges, then copy the flags bitfield word from
 * `that` (preserving whatever `HCEX_OBJ_FLAGS(obj)` already held in the reserved low bits, since the word
 * is otherwise uninitialized on entry).
 *
 * Modeled as a free helper (see HCEX_OBJ__HCEX_OBJ_default.cpp for why). followers/followersOfs
 * are reinterpret_cast to their real, already-reversed dsVECTOR<T,8> template instantiations
 * (dsVECTOR_WEAK_PTR_entENTITY_8__Insert.cpp / the m3dMATR sibling) — dsVECTOR_followers and
 * dsVECTOR<ds::WEAK_PTR<entENTITY>,8> share the identical pData/nElem/allocated/__cl layout.
 *
 * DEVIATION (followersOfs.Insert call): the decompiler fabricates ~15 extra trailing integer
 * params on this call (v7..v31) — the classic float-slot-skip decompiler artifact from an
 * m3dMATR-heavy instantiation's ABI; the real signature is dsVECTOR<m3dMATR,8>::Insert(int pos,
 * const m3dMATR *begin, int count), matching the template's declared 3-arg overload.
 *
 * DEVIATION (flags): the decompiler unrolls the bitfield copy as ~13 sequential single-bit
 * read-modify-write ops (one per named flag bit, isFuncDin..isInited). Verified equivalent to a
 * single masked copy of bits 17..31 from `HCEX_OBJ_FLAGS(that)` (the reserved low 17 bits are left as
 * whatever `HCEX_OBJ_FLAGS(obj)` already held, since the dword is otherwise never initialized here). */

#include "../../headers/hcex/HCEX_OBJ.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/WEAK_PTR.h"
#include "../../headers/ws/m3d/m3dMATR.h"

struct entENTITY; // boundary — referenced only through the weak handle

void hcex_obj_copy_construct(HCEX_OBJ *obj, const HCEX_OBJ *that)
{
    obj->id = that->id;
    obj->pInst = that->pInst;
    obj->modelIdx = that->modelIdx;

    obj->followers.pData = 0;
    obj->followers.nElem = 0;
    obj->followers.allocated = 0;
    obj->followers.__cl = that->followers.__cl;
    reinterpret_cast<dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> &>(obj->followers)
        .Insert(0,
                reinterpret_cast<const ds::WEAK_PTR<entENTITY> *>(that->followers.pData),
                that->followers.nElem);

    obj->followersOfs.pData = 0;
    obj->followersOfs.nElem = 0;
    obj->followersOfs.allocated = 0;
    obj->followersOfs.__cl = that->followersOfs.__cl;
    reinterpret_cast<dsVECTOR<m3dMATR, 8> &>(obj->followersOfs)
        .Insert(0,
                reinterpret_cast<const m3dMATR *>(that->followersOfs.pData),
                that->followersOfs.nElem); // DEVIATION: see header comment

    HCEX_OBJ_FLAGS(obj) = (HCEX_OBJ_FLAGS(obj) & 0x1FFFFu) | (HCEX_OBJ_FLAGS(that) & 0xFFFE0000u); // DEVIATION: see header comment
}
