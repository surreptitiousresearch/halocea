#include "../../headers/ws/ai/aiWEAPON.h"
#include "../../headers/ws/fsm/fsmWPN_LIST_PARAM_WEAPON.h"
#include "../../headers/ws/wpn/wpnLIST_BASE_boundary.h"
#include "../../headers/ws/ia/iaIACTOR.h"
#include "../../headers/ws/prop/propCONTAINER.h"
#include "../../headers/ws/prop/propCONTAINER_DESC.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE.h"
#include "../../headers/ws/prop/propWPN_AI_USABLE_DESC.h"

// aiWEAPON::FetchCtrl @ 0x83294DD8
// ?FetchCtrl@aiWEAPON@@IAAPAVpropWPN_AI_USABLE@@PAUfsmWPN_LIST_PARAM_WEAPON@@@Z
//
// Locate the AI-usable weapon controller that owns the weapon named in `par`: inspect the weapon's
// owning actor's property container. If the container itself is (derived from) a propWPN_AI_USABLE,
// it is the controller; otherwise, when it is a real property collection, scan its properties then
// its components for the first propWPN_AI_USABLE. Returns null if none is found.
propWPN_AI_USABLE *aiWEAPON::FetchCtrl(fsmWPN_LIST_PARAM_WEAPON *par)
{
    if (!par || !par->wpn)
        return nullptr;

    iaIACTOR *pOwner = par->wpn->pOwner;
    if (!pOwner)
        return nullptr;

    propCONTAINER &container = pOwner->propContainer;
    if (container.typeId.IsDerivedFrom(propWPN_AI_USABLE_DESC::TYPE_ID))
        return (propWPN_AI_USABLE *)&container;

    if (!container.typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID))
        return nullptr;

    for (int i = 0; i < container.properties.nElem; ++i) {
        propWPN_AI_USABLE *found = container.properties[i]->GetProperty<propWPN_AI_USABLE>();
        if (found)
            return found;
    }
    for (int i = 0; i < container.components.nElem; ++i) {
        propWPN_AI_USABLE *found = container.components[i]->GetProperty<propWPN_AI_USABLE>();
        if (found)
            return found;
    }
    return nullptr;
}
