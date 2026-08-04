#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// 0x832311A8  aiCOORD_GROUP::cbFilterWPSetByAngleVsTarget
// ssl callback: retrieve the float arg (argv[1]) and the caller-host actor (argv[0]),
// dispatch FilterWPSetByAngleVsTarget, return the bool.
void aiCOORD_GROUP::cbFilterWPSetByAngleVsTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    float floatArg;
    argv[1].GetValue<float>(floatArg, DSD_CONV_RETRIEVE);

    sslOBJ_REF ref;
    ref.Set(*argv, gsSslSystem);
    iaIACTOR *host = reinterpret_cast<iaIACTOR *>(ref.GetCbHost());

    bool result = this->FilterWPSetByAngleVsTarget(host, floatArg);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
