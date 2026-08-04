#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"

// 0x83230550  aiCOORD_GROUP::cbHaveWPSet
// ssl callback: dispatch HaveWPSet (no script args) and return the result.
void aiCOORD_GROUP::cbHaveWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;
    bool result = this->HaveWPSet();

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
