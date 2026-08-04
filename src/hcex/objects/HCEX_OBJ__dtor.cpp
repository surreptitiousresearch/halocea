/* HCEX_OBJ dtor @0x823EDA80 — free followersOfs's backing storage directly (dlFree, no
 * per-element destructor since m3dMATR is trivially destructible) and run the real
 * dsVECTOR<ds::WEAK_PTR<entENTITY>,8> teardown on followers (releases each weak handle, then
 * frees the backing storage) via its already-reversed Clear() instantiation.
 *
 * Modeled as a free helper (see HCEX_OBJ__HCEX_OBJ_default.cpp for why). */

#include "../../headers/hcex/HCEX_OBJ.h"
#include "../../headers/ws/ds/dsVECTOR.h"
#include "../../headers/ws/ds/WEAK_PTR.h"

struct entENTITY; // boundary — referenced only through the weak handle

extern "C" void dlFree(void *ptr); // boundary — dlmalloc free

void hcex_obj_destruct(HCEX_OBJ *obj)
{
    dlFree(obj->followersOfs.pData);
    dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> &followers =
        reinterpret_cast<dsVECTOR<ds::WEAK_PTR<entENTITY>, 8> &>(obj->followers);
    followers.Clear();
    dlFree(followers.pData);
}
