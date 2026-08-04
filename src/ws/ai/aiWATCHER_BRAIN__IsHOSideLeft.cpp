#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsHOSideLeft @ 0x8316F5E0
// ?IsHOSideLeft@aiWATCHER_BRAIN@@UBA_NXZ
// Virtual const: true when the brain body's cached hideout-position property set marks the unit on
// the left (outer) side of its hideout. Only meaningful in hideout mode and not while wall-leaning.
//
// The test is a bit-subset check: the required property set (LEFT | OUTER) must be fully present in
// the body's cached property set. The binary walks the odd-indexed cells (1,3,5,7,9,11) of the
// 12-cell fixed bit array, requiring every required bit to be set in the cache.
bool aiWATCHER_BRAIN::IsHOSideLeft() const
{
    if (!IsHideoutMode() || IsWlnMode())
        return false;

    const aiBODY_IFACE *body = brain.GetPtr()->body.pointee;
    ctrlPROP_LIST required = HO_POS_LEFT | HO_POS_OUTER;
    const ctrlPROP_LIST *cache = body->GetRefPropCache();

    int matched = 0;
    for (int cell = 1; cell < 12; cell += 2)
    {
        if (required.props.array[cell] != (cache->props.array[cell] & required.props.array[cell]))
            return false;
        if (++matched >= 6)
            return true;
    }
    return false;
}
