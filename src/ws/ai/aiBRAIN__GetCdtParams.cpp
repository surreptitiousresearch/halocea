#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/mdl/mdlINFO_BASE.h"
#include "../../headers/ws/mdl/mdlCDT_SETTINGS.h"

// 0x832ABC68 ?GetCdtParams@aiBRAIN@@QBAABVmdlCDT_SETTINGS@@XZ
// The brain's collision-detection tuning: from the body's model-info descriptor when the body has
// model info, else a lazily-initialised process-wide default block.
const mdlCDT_SETTINGS *aiBRAIN::GetCdtParams() const
{
    // Field order matches mdlCDT_SETTINGS.h:
    //   radCdt, radCdtAnim, heightCdtAnim, radCdtSafe, checkFloorCrash,
    //   charPhysWeight, wpnPointDistMin, neverSkipBodies, distSkipFloor
    static mdlCDT_SETTINGS defaults =
    {
        0.5f,   // radCdt
        0.5f,   // radCdtAnim
        1.5f,   // heightCdtAnim
        0.2f,   // radCdtSafe
        true,   // checkFloorCrash
        0.0f,   // charPhysWeight
        0.0f,   // wpnPointDistMin
        false,  // neverSkipBodies
        0.0f,   // distSkipFloor
    };

    mdlINFO_BASE *info = this->body.pointee->GetBodyInfo();
    if (info)
        return &info->desc->cdtSettings;

    return &defaults;
}
