#include "../../headers/ws/ai/aiWATCHER_BRAIN.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWATCHER_BRAIN_boundaries.h"

// aiWATCHER_BRAIN::IsHOSideRight @ 0x8316F6E0
// ?IsHOSideRight@aiWATCHER_BRAIN@@UBA_NXZ
// Virtual const: true when the brain body's cached hideout-position property set marks the unit on
// the right (outer) side of its hideout. Identical subset test to IsHOSideLeft with the RIGHT id.
bool aiWATCHER_BRAIN::IsHOSideRight() const
{
    if (!IsHideoutMode() || IsWlnMode())
        return false;

    const aiBODY_IFACE *body = brain.GetPtr()->body.pointee;
    ctrlPROP_LIST required = HO_POS_RIGHT | HO_POS_OUTER;
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
