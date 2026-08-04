#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"

// 0x83230270  aiCOORD_GROUP::cbGetTargetMovementDeltaDist
// ssl callback: dispatch GetTargetMovementDeltaDist (no script args) and return the result.
void aiCOORD_GROUP::cbGetTargetMovementDeltaDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;
    float result = this->GetTargetMovementDeltaDist();

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<float>(result);
    retVal.StoreValue(out);
}
