#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ia/iaIACTOR.h"             // propContainer + typeId
#include "../../headers/ws/prop/propCONTAINER.h"      // properties / components vectors
#include "../../headers/ws/prop/propBASE.h"           // propBASE::GetProperty<>
#include "../../headers/ws/prop/propCONTAINER_DESC.h" // propCONTAINER_DESC::TYPE_ID
#include "../../headers/ws/ai/aiPROP_FIREABLE.h"      // aiPROP_FIREABLE::Die(nameSeq)
#include "../../headers/ws/ai/aiPROP_FIREABLE_DESC_boundary.h" // aiPROP_FIREABLE_DESC::TYPE_ID
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::Die(const dsTSTRING<char>&) @ 0x832AE528
// ?Die@aiBRAIN@@UAAXABV?$dsTSTRING@D@@@Z
//
// Scripted death by kill-word `reason`. Locate the brain's fireable property (either the property
// container itself IS an aiPROP_FIREABLE, or it holds one among its properties/components) and
// forward to its scripted-death handler. Same fireable-lookup as GetHealthPercent/UpdateFlags.
void aiBRAIN::Die(const dsTSTRING<char> &reason)
{
    propCONTAINER *pc = &propContainer;
    dsTYPE_ID *typeId = &pc->typeId;

    aiPROP_FIREABLE *fireable = nullptr;
    if (typeId->IsDerivedFrom(aiPROP_FIREABLE_DESC::TYPE_ID))
    {
        fireable = reinterpret_cast<aiPROP_FIREABLE *>(pc);
    }
    else if (typeId->IsDerivedFrom(propCONTAINER_DESC::TYPE_ID))
    {
        for (int i = 0; i < pc->properties.nElem && !fireable; ++i)
            fireable = pc->properties[i]->GetProperty<aiPROP_FIREABLE>();
        for (int i = 0; i < pc->components.nElem && !fireable; ++i)
            fireable = pc->components[i]->GetProperty<aiPROP_FIREABLE>();
    }

    fireable->Die(reason);
}
