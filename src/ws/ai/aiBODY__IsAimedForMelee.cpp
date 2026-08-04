#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ds/dsBIT_ARRAY_IMPL.h"
#include "../../headers/ws/ai/aiWATCHER_props_boundary.h"

// 0x832AB998 ?IsAimedForMelee@aiBODY@@UBA_NXZ
// True when the body's model-state cache reports procedural aim is not clamped (melee-aligned).
bool aiBODY::IsAimedForMelee() const
{
    const ctrlPROP_LIST *cache = this->GetRefPropCache();
    return dsBIT_ARRAY_IMPL::Is(MDL_ST_FLAG_IS_PROC_AIM_NOT_CLAMPED.id, cache->props.array, 12) != 0;
}
