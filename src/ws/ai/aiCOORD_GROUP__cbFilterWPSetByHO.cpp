#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// 0x83230F90  aiCOORD_GROUP::cbFilterWPSetByHO
// ssl callback: retrieve the bool script arg, dispatch FilterWPSetByHO, return the bool.
void aiCOORD_GROUP::cbFilterWPSetByHO(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;
    bool boolArg;
    argv[0].GetValue<bool>(boolArg, DSD_CONV_RETRIEVE);

    bool result = this->FilterWPSetByHO(boolArg);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
