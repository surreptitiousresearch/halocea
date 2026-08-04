#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"

// 0x832307E0  aiCOORD_GROUP::cbGetWPSetFromNavSys
// ssl callback: dispatch GetWPSetFromNavSys (no script args) and return the result.
void aiCOORD_GROUP::cbGetWPSetFromNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;
    bool result = this->GetWPSetFromNavSys();

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
