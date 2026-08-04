#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// aiCOORD_GROUP::cbAddPossibleEnemiesFromTarget @ 0x83231780
// ?cbAddPossibleEnemiesFromTarget@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: resolve the caller-host actor from argv[0] and dispatch AddPossibleEnemiesFromTarget.
void aiCOORD_GROUP::cbAddPossibleEnemiesFromTarget(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)retVal; (void)caller;

    sslOBJ_REF host;
    host.Set(*argv, gsSslSystem);
    iaIACTOR *cbHost = reinterpret_cast<iaIACTOR *>(host.GetCbHost());

    this->AddPossibleEnemiesFromTarget(cbHost);
}
