#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"

// 0x832305E0  aiCOORD_GROUP::cbGetWPSetCount
// ssl callback: dispatch GetWPSetCount (no script args) and return the result.
void aiCOORD_GROUP::cbGetWPSetCount(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;
    int result = this->GetWPSetCount();

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<int>(result);
    retVal.StoreValue(out);
}
