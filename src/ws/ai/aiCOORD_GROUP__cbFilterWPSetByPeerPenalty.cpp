#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// 0x83230EB0  aiCOORD_GROUP::cbFilterWPSetByPeerPenalty
// ssl callback: resolve the caller-host actor (argv[0]), dispatch FilterWPSetByPeerPenalty, return the bool.
void aiCOORD_GROUP::cbFilterWPSetByPeerPenalty(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    sslOBJ_REF ref;
    ref.Set(*argv, gsSslSystem);
    iaIACTOR *host = reinterpret_cast<iaIACTOR *>(ref.GetCbHost());

    bool result = this->FilterWPSetByPeerPenalty(host);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
