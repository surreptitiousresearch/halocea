#include "../../headers/ws/ai/aiPROP_FIREABLE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/animINST.h"

// aiPROP_FIREABLE::Revitalize() @ 0x832A38D0
// ?Revitalize@aiPROP_FIREABLE@@UAAXXZ (virtual)
// Restore a downed AI: run the base revitalize, clear the owner's BLEED death bit, and reset the
// model's collision layer if it was in the "empty" state.
void aiPROP_FIREABLE::Revitalize()
{
    propFIREABLE::Revitalize();

    aiBRAIN *owner = (aiBRAIN *)this->pOwner; // pOwner is concretely an aiBRAIN
    owner->st.pointee->die.val &= ~2;         // clear AI_DIEST_BLEED

    animINST *inst = owner->pInst;
    if (inst->cdtLayer == 0x4000)
    {
        inst->cdtLayer = 128;
        inst->ClearIGNORE_CDT_ALWAYS();
    }
}
