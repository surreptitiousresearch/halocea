#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// 0x83230470  aiCOORD_GROUP::cbGetDistToTargetViaNavSys
// ssl callback: resolve the callback host actor from argv[0], dispatch GetDistToTargetViaNavSys, return the result.
void aiCOORD_GROUP::cbGetDistToTargetViaNavSys(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;
    sslOBJ_REF host;
    host.Set(*argv, gsSslSystem);
    iaIACTOR *cbHost = (iaIACTOR *)host.GetCbHost();

    float result = this->GetDistToTargetViaNavSys(cbHost);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<float>(result);
    retVal.StoreValue(out);
}
