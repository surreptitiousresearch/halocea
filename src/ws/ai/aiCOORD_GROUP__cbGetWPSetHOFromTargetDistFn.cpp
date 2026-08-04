#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// 0x832309C8  aiCOORD_GROUP::cbGetWPSetHOFromTargetDistFn
// ssl callback: retrieve the int script arg, dispatch GetWPSetHOFromTargetDistFn, return the bool.
void aiCOORD_GROUP::cbGetWPSetHOFromTargetDistFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;
    int intArg;
    argv[0].GetValue<int>(intArg, DSD_CONV_RETRIEVE);

    bool result = this->GetWPSetHOFromTargetDistFn(intArg);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
