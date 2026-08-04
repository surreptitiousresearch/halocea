#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// aiCOORD_GROUP::cbFilterWPSetByDistToEnemyLessDist
// ?cbFilterWPSetByDistToEnemyLessDist@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: resolve the caller-host actor from argv[0] and a distance from argv[1],
// dispatch FilterWPSetByDistToEnemyLessDist, and store the bool result.
void aiCOORD_GROUP::cbFilterWPSetByDistToEnemyLessDist(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    float dist = 0.0f;
    (argv + 1)->GetValue<float>(dist, DSD_CONV_RETRIEVE);

    sslOBJ_REF host;
    host.Set(*argv, gsSslSystem);
    iaIACTOR *cbHost = reinterpret_cast<iaIACTOR *>(host.GetCbHost());

    bool result = this->FilterWPSetByDistToEnemyLessDist(cbHost, dist);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
}
