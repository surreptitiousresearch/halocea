/* ?AddLightSet@animINST@@QAAXV?$dsSMART_PTR@VrendLIGHT_SET@@V?$_dsSP_OWNER_PLC_REFCOUNT@PAVdsREF_COUNT@@@@@@@Z @0x825E4068 */
#include "animINST.h"
#include "../ds/dsSMART_PTR.h"
#include "../ds/dsREF_COUNT.h"
#include "../rend/rendLIGHT_SET.h"
#include <new>

typedef dsSMART_PTR<rendLIGHT_SET, _dsSP_OWNER_PLC_REFCOUNT<dsREF_COUNT *> > LIGHT_SET_SP;

// Debug placement operator new the ws build links in. boundary.
void *operator new(size_t size, const char *file, unsigned int line);

// anim.cpp helper: hand the object subtree its new light set, releasing the previous one.
// Takes both smart pointers by value (the caller passes freshly ref-bumped copies). boundary.
void animReplaceLightSet(objOBJ *pObj, LIGHT_SET_SP oldSet, LIGHT_SET_SP newSet); // boundary

// ?AddLightSet@animINST@@QAAXV?$dsSMART_PTR@VrendLIGHT_SET@@...@@@Z  (0x825E4068) — attach a light
// set to this instance. If `pLightSet` carries a referent, adopt it (swapping it onto the object
// subtree). Otherwise, if the instance has none yet, lazily create one. Either way the final set
// is told which instance owns it. The passed-in smart pointer is released on the way out.
void animINST::AddLightSet(LIGHT_SET_SP pLightSet)
{
    LIGHT_SET_SP *pMember = reinterpret_cast<LIGHT_SET_SP *>(&this->pLightSet);

    if (pLightSet.pointee)
    {
        // Replace the current set on the object subtree with the incoming one (both copied).
        animReplaceLightSet(this->pObj, *pMember, pLightSet);
        *pMember = pLightSet;
    }
    else if (!pMember->pointee)
    {
        rendLIGHT_SET *pNew = (rendLIGHT_SET *)operator new(
            0x268u, "D:\\Projects\\code\\common\\src.sys\\animation\\anim.cpp", 0x6ABu);
        if (pNew)
            new (pNew) rendLIGHT_SET(this, nullptr);

        LIGHT_SET_SP created;
        created.pointee = pNew;
        *pMember = created;
        created.DeletePointee();

        // A paused-but-not-suppressed instance starts its fresh light set active (state bit 0).
        if ((this->state & 0x40) != 0 && (this->state2 & 0x40) == 0)
            pMember->pointee->state.state |= 1u;
    }

    if (pMember->pointee)
        pMember->pointee->SetInstOwner(this);

    pLightSet.DeletePointee();
}
