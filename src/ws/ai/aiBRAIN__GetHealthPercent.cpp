#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ia/iaIACTOR.h"        // propContainer + typeId
#include "../../headers/ws/prop/propCONTAINER.h" // properties / components vectors
#include "../../headers/ws/prop/propBASE.h"      // propBASE::GetProperty<>
#include "../../headers/ws/prop/propCONTAINER_DESC.h" // propCONTAINER_DESC::TYPE_ID
#include "../../headers/ws/ai/aiPROP_FIREABLE.h" // aiPROP_FIREABLE + Hp() (via propFIREABLE base)
#include "../../headers/ws/ai/aiPROP_FIREABLE_DESC_boundary.h" // aiPROP_FIREABLE_DESC::TYPE_ID
#include "../../headers/ws/ds/dsTYPE_ID.h"

// aiBRAIN::GetHealthPercent() @ 0x832AE3D8
// ?GetHealthPercent@aiBRAIN@@QBAMXZ
//
// Current health as a 0..1 fraction (hit points * 0.01), or 0 when dead. Finds the brain's fireable
// property: either the property container itself IS an aiPROP_FIREABLE, or it is a container that
// holds one among its properties/components.
float aiBRAIN::GetHealthPercent() const
{
    if (IsDead())
        return 0.0f;

    // DEVIATION: the DB-const accessor scans/queries the property container non-const; cast away
    // const to match the decompiler's mutable access (no state is actually mutated here).
    propCONTAINER *pc = const_cast<propCONTAINER *>(&propContainer);
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

    return static_cast<float>(fireable->Hp()) * 0.0099999998f;
}
