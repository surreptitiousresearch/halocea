#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ds/dsBIT_ARRAY_IMPL.h"
#include "../../headers/ws/ai/aiWATCHER_props_boundary.h"

// 0x832AB9E0 ?GetShootingResponse@aiBODY@@UBAHXZ
// 1 = shooting anim active, 2 = shooting anim disabled, 0 = neither.
int aiBODY::GetShootingResponse() const
{
    const ctrlPROP_LIST *cache = this->GetRefPropCache();
    if (dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_SHOOTING_ANIM.id, cache->props.array, 12))
        return 1;

    const ctrlPROP_LIST *cache2 = this->GetRefPropCache();
    return dsBIT_ARRAY_IMPL::Is(MDL_ST_PROP_DISABLED_SHOOTING_ANIM.id, cache2->props.array, 12) == 0 ? 0 : 2;
}
