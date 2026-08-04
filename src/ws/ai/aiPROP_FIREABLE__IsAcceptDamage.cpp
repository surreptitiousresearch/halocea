#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiPROP_FIREABLE_boundaries.h"
#include "../../headers/ws/prop/propFIREABLE_DESC.h"
#include "../../headers/ws/gs/gsGEOM_ANCHOR.h"
#include "../../headers/ws/ds/dsCMP.h"

// aiPROP_FIREABLE::IsAcceptDamage() @ 0x832A3B88
// ?IsAcceptDamage@aiPROP_FIREABLE@@UAA_NPBVdmgTYPE@@ABVgsGEOM_ANCHOR@@H@Z  (vtbl)
//
// Reject all damage under god-mode or a descriptor-level reject; otherwise, when a per-object
// fireable filter list is present, accept only hits against a listed object.
bool aiPROP_FIREABLE::IsAcceptDamage(const dmgTYPE *pDmgType, const gsGEOM_ANCHOR &hitInfo, int isDirectHit)
{
    if (aiDEBUG_IsGodMode()
        || !((propFIREABLE_DESC *)this->spDesc.pointee)->IsAcceptDamage(
               pDmgType, const_cast<gsGEOM_ANCHOR *>(&hitInfo), isDirectHit))
    {
        return false;
    }

    if (this->objFireable.nElem <= 0)
        return true;

    objOBJ *Obj = gsGEOM_ANCHOR_GetObj(&hitInfo);
    dsCMP cmp = {};
    // accept iff the hit object is present in the fireable filter list
    return this->objFireable.Find(Obj, cmp, 0) >= 0;
}
