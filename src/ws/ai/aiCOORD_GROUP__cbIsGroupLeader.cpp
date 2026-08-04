#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// aiCOORD_GROUP::cbIsGroupLeader @ 0x8322FE68
// ?cbIsGroupLeader@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: report whether the caller-host actor (argv[0]) is the group's leader.
void aiCOORD_GROUP::cbIsGroupLeader(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)caller;

    sslOBJ_REF ref;
    ref.Set(*argv, gsSslSystem);
    iaIACTOR *host = reinterpret_cast<iaIACTOR *>(ref.GetCbHost());

    bool result = IsGroupLeader(host);

    dsDATA tmp;
    tmp.type = nullptr;
    tmp.StoreValue<bool>(result);
    retVal.StoreValue(tmp);
}
