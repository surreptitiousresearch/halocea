#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ia/iaIACTOR.h"             // propContainer + typeId
#include "../../headers/ws/prop/propCONTAINER.h"      // properties / components vectors
#include "../../headers/ws/prop/propBASE.h"           // propBASE::GetProperty<>
#include "../../headers/ws/prop/propCONTAINER_DESC.h" // propCONTAINER_DESC::TYPE_ID
#include "../../headers/ws/ai/aiPROP_FIREABLE.h"      // aiPROP_FIREABLE::Die(nameSeq)
#include "../../headers/ws/ai/aiPROP_FIREABLE_DESC_boundary.h" // aiPROP_FIREABLE_DESC::TYPE_ID
#include "../../headers/ws/ds/dsTYPE_ID.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::UpdateFlags() @ 0x832AEA48
// ?UpdateFlags@aiBRAIN@@IAAXXZ  (protected)
//
// Fold pending per-frame status requests into behaviour: when the death-state ragdoll bit
// (AI_DIEST bit 20) is set, clear it and drive the fireable property's scripted death with the
// "RAGDOLL" kill-word. Then refresh the mind sparser and, when the frame is not sparsed and in-game
// AI debug text is being rendered, run the per-frame debug-text hooks.
void aiBRAIN::UpdateFlags()
{
    aiSTATUS *status = st.pointee;
    unsigned int dieFlags = status->die.val;
    if ((dieFlags >> 20) & 1)
    {
        status->die.val = dieFlags & 0xFFEFFFFF; // clear the ragdoll-request bit

        // Same fireable lookup as GetHealthPercent/Die.
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

        if (fireable)
        {
            dsTSTRING<char> ragdoll("RAGDOLL");
            fireable->Die(ragdoll);
        }
    }

    mind.UpdateSparser();

    if ((st.pointee->mind.val & 1) == 0 && aiDEBUG::IsRenderIngameText())
    {
        OnDbgOut();
        OnFrameBegin();
    }
}
