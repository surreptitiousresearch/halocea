#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_IFACE.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/dmg/dmgDAMAGE.h"
#include "../../headers/ws/dmg/dmgTYPE.h"
#include "../../headers/ws/prop/propGAME_INFO_TEAM.h"
#include "../../headers/ws/ds/WEAK_PTR.h"
#include "../../headers/ws/ds/dsSTRID.h"

// ws RTTI predicate (declared per-instantiation at the call site).  boundary.
template<class T> int gsIs(const void *obj);
// Global damage-type name id for explosive damage (?DMG_TYPE_NAME_EXPLOSIVE@@3VdsSTRID@@A @0x84326D1C).
extern const dsSTRID DMG_TYPE_NAME_EXPLOSIVE;

// aiPROP_FIREABLE::IsIgnoring(const dmgDAMAGE*) const @ 0x832A39C8
// ?IsIgnoring@aiPROP_FIREABLE@@IBA_NPBVdmgDAMAGE@@@Z (protected const)
// Decide whether this AI unit should ignore an incoming damage event.
bool aiPROP_FIREABLE::IsIgnoring(const dmgDAMAGE *pDmg) const
{
    if (this->isSkipRevitalize)
        return false;
    if (aiDEBUG::IsGodMode())
        return true;

    bool isExplosive = pDmg && pDmg->pDmgType
                    && pDmg->pDmgType->name.id == DMG_TYPE_NAME_EXPLOSIVE.id;

    // Explosive hits and friendly fire are never ignored here.
    if (isExplosive || this->IsFriendlyFire())
        return false;

    aiBRAIN_IFACE *shooter = pDmg->params.entShooter.pHandle
                           ? (aiBRAIN_IFACE *)pDmg->params.entShooter.pHandle->pPtr
                           : nullptr;
    if (!gsIs<aiBRAIN>(shooter))
        return false;

    aiBRAIN *owner = (aiBRAIN *)this->pOwner; // pOwner is concretely an aiBRAIN
    if ((void *)owner == (void *)shooter)
        return true;

    if (shooter->GetTeamID()->team.id != ((aiBRAIN_IFACE *)owner)->GetTeamID()->team.id)
        return false;

    // Same team: ignore only when the owner's general-state "accept friendly damage" bit is clear.
    return ((~owner->st.pointee->gen.val >> 21) & 1) != 0;
}
