#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// 0x83230B40  aiCOORD_GROUP::cbFilterWPSetByTargetAssocDistFn
// ssl callback: retrieve (int, bool, float) script args, dispatch FilterWPSetByTargetAssocDistFn,
// return the bool result.
void aiCOORD_GROUP::cbFilterWPSetByTargetAssocDistFn(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;
    float distArg;
    argv[2].GetValue<float>(distArg, DSD_CONV_RETRIEVE);
    bool boolArg;
    argv[1].GetValue<bool>(boolArg, DSD_CONV_RETRIEVE);
    int intArg;
    argv[0].GetValue<int>(intArg, DSD_CONV_RETRIEVE);

    bool result = this->FilterWPSetByTargetAssocDistFn(intArg, boolArg, distArg);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
