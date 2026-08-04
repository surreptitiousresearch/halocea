#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"

// 0x83230670  aiCOORD_GROUP::cbEnsureHaveWPSet
// ssl callback: dispatch EnsureHaveWPSet (no script args) and return the result.
void aiCOORD_GROUP::cbEnsureHaveWPSet(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)argv; (void)caller;
    bool result = this->EnsureHaveWPSet();

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
