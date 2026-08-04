#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// 0x83230D28  aiCOORD_GROUP::cbFilterWPSetByMaxDistToTarget
// ssl callback: retrieve the float script arg, dispatch FilterWPSetByMaxDistToTarget, return the bool.
void aiCOORD_GROUP::cbFilterWPSetByMaxDistToTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;
    float distArg;
    argv[0].GetValue<float>(distArg, DSD_CONV_RETRIEVE);

    bool result = this->FilterWPSetByMaxDistToTarget(distArg);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
