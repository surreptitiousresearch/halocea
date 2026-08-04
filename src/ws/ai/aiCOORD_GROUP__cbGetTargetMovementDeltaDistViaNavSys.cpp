#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"

// 0x83230300  aiCOORD_GROUP::cbGetTargetMovementDeltaDistViaNavSys
// ssl callback: dispatch GetTargetMovementDeltaDistViaNavSys (no script args) and return the result.
void aiCOORD_GROUP::cbGetTargetMovementDeltaDistViaNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;
    float result = this->GetTargetMovementDeltaDistViaNavSys();

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<float>(result);
    retVal.StoreValue(out);
}
