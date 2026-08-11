/* HCEX_OBJ::`scalar deleting destructor' @0x823EDBD0 — same teardown as the HCEX_OBJ dtor, then
 * conditionally operator-deletes `obj` when bit 0 of `deleteFlags` is set (compiler-generated
 * `??_G` thunk).
 *
 * Modeled as a free helper (see HCEX_OBJ__HCEX_OBJ_default.cpp for why). */

#include "../../headers/hcex/HCEX_OBJ.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/WEAK_PTR.h"

struct entENTITY; // boundary — referenced only through the weak handle

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

HCEX_OBJ *hcex_obj_scalar_deleting_dtor(HCEX_OBJ *obj, unsigned char deleteFlags)
{
    dlFree(obj->followersOfs.pData);
    dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> &followers =
        reinterpret_cast<dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> &>(obj->followers);
    followers.Clear();
    dlFree(followers.pData);

    if ( deleteFlags & 1 )
        operator delete(obj);
    return obj;
}
