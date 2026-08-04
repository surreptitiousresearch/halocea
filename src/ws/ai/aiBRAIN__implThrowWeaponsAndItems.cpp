// aiBRAIN::implThrowWeaponsAndItems @ 0x8325B930
// ?implThrowWeaponsAndItems@aiBRAIN@@UAAXXZ
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBRAIN_throwWeaponsAndItems_boundary.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/prop/propBASE.h"

extern aiPLANNER *aiPlanner; // ai08 process-wide planner
extern void _apLog(const char *fmt, ...); // ws/ap — engine log  boundary

// death-state flag: a ThrowWeaponsAndItems request has already been serviced this life.
static const int AI_DIE_THREW_WEAPONS = 0x8000;
// planner-state flag: AI simulation is off (skip the throw).
static const int AI_PLAST_AI_OFF = 0x10;

// Resolve the P-typed property from this brain's prop container: if the container itself is a P, use
// it; otherwise, when it owns a property collection, scan properties[] then components[].
template<class P, class DESC>
static P *FindProp(propCONTAINER &container)
{
    if (container.typeId.IsDerivedFrom(DESC::TYPE_ID))
        return reinterpret_cast<P *>(&container);
    if (!container.typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID))
        return nullptr;
    for (int i = 0; i < container.properties.nElem; ++i) {
        P *found = container.properties[i]->GetProperty<P>();
        if (found)
            return found;
    }
    for (int i = 0; i < container.components.nElem; ++i) {
        P *found = container.components[i]->GetProperty<P>();
        if (found)
            return found;
    }
    return nullptr;
}

// Virtual (SSL "ThrowWeaponsAndItems"): mark the throw as serviced, then have the unit drop all held
// weapons and all carried items. No-op if already serviced or while AI is switched off.
void aiBRAIN::implThrowWeaponsAndItems()
{
    aiSTATUS *status = st.pointee;
    if (status->die.val & AI_DIE_THREW_WEAPONS)
        return;
    status->die.val |= AI_DIE_THREW_WEAPONS;

    if (aiPlanner && (aiPlanner->state.val & AI_PLAST_AI_OFF))
        return;

    _apLog("~AI,Dbgout,Ssl~'%s': ssl call ThrowWeaponsAndItems", names.nameInst);

    wpnLIST *weapons = FindProp<wpnLIST, wpnLIST_DESC>(propContainer);
    if (weapons)
        weapons->ThrowAllWeapons();

    wpnITEM_LIST *items = FindProp<wpnITEM_LIST, wpnITEM_LIST_DESC>(propContainer);
    if (items)
        items->implThrowAllItems();
}
