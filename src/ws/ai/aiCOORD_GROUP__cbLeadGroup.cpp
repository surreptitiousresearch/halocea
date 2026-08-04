#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// aiCOORD_GROUP::cbLeadGroup @ 0x8322F9B8
// ?cbLeadGroup@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: make the caller-host actor (argv[0]) lead the group; store the bool result.
void aiCOORD_GROUP::cbLeadGroup(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    sslOBJ_REF ref;
    ref.Set(*argv, gsSslSystem);
    iaIACTOR *host = reinterpret_cast<iaIACTOR *>(ref.GetCbHost());

    bool result = LeadGroup(host);

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(result);
    retVal.StoreValue(tmp);
}
