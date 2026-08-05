// 0x823E1E90  GetModifier  (unmangled free function; owner is the global hcex fp-model bridge,
// NOT a member of objOBJ). Locate the first-person model matching (modelId, localPlayerIndex),
// then resolve its entity's propOBJ_MODIFIER_LIST via the same propCONTAINER scan pattern as
// iaIACTOR::GetProperty<propOBJ_MODIFIER_LIST>: if the entity's propContainer itself derives from
// the modifier-list DESC, it IS the list; otherwise, only when it derives from propCONTAINER_DESC,
// linear-scan properties[] then components[] returning the first non-null GetProperty<> hit.
//
// The decompiler shows spurious trailing register args (a3..a8) on GetModifier and on each
// dsVECTOR::operator[]; the real signatures take a single index. Verified against disasm.
#include <stdint.h>
#include "../headers/fp_model_vector.h"
#include "../headers/ws/ent/entENTITY.h"
#include "../headers/ws/prop/propOBJ_MODIFIER_LIST.h"
#include "../headers/ws/prop/propOBJ_MODIFIER_LIST_DESC.h"
#include "../headers/ws/prop/propCONTAINER_DESC.h"

extern dsVECTOR<FP_MODEL, 8> fpModels;

propOBJ_MODIFIER_LIST *GetModifier(int modelId, int16_t localPlayerIndex)
{
    int idx = 0;
    if (fpModels.nElem <= 0)
        return nullptr;

    while (true) {
        if (fpModels[idx].modelId == modelId && fpModels[idx].plrIdx == localPlayerIndex)
            break;
        if (++idx >= fpModels.nElem)
            return nullptr;
    }

    // (The reversed body re-null-checks the operator[] result; with a reference it cannot be null.)
    entENTITY *pEnt = fpModels[idx].pInst->pEnt;
    propCONTAINER &container = pEnt->propContainer;

    if (container.typeId.IsDerivedFrom(propOBJ_MODIFIER_LIST_DESC::TYPE_ID))
        return (propOBJ_MODIFIER_LIST *)&container;

    if (!container.typeId.IsDerivedFrom(propCONTAINER_DESC::TYPE_ID))
        return nullptr;

    for (int i = 0; i < container.properties.nElem; ++i) {
        propOBJ_MODIFIER_LIST *found = container.properties[i]->GetProperty<propOBJ_MODIFIER_LIST>();
        if (found)
            return found;
    }
    for (int i = 0; i < container.components.nElem; ++i) {
        propOBJ_MODIFIER_LIST *found = container.components[i]->GetProperty<propOBJ_MODIFIER_LIST>();
        if (found)
            return found;
    }
    return nullptr;
}
