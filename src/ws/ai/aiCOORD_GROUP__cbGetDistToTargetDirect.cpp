#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// 0x83230390  aiCOORD_GROUP::cbGetDistToTargetDirect
// ssl callback: resolve the callback host actor from argv[0], dispatch GetDistToTargetDirect, return the result.
void aiCOORD_GROUP::cbGetDistToTargetDirect(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;
    sslOBJ_REF host;
    host.Set(*argv, gsSslSystem);
    iaIACTOR *cbHost = (iaIACTOR *)host.GetCbHost();

    float result = this->GetDistToTargetDirect(cbHost);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<float>(result);
    retVal.StoreValue(out);
}
