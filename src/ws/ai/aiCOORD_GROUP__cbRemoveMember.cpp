#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // global ssl script system

// aiCOORD_GROUP::cbRemoveMember @ 0x8322F920
// ?cbRemoveMember@aiCOORD_GROUP@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL callback: resolve the caller-host actor from argv[0] and remove it from the group.
void aiCOORD_GROUP::cbRemoveMember(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    (void)argc; (void)retVal; (void)caller;

    sslOBJ_REF ref;
    ref.Set(*argv, gsSslSystem); // returned sslERROR discarded (its desc string self-releases)
    iaIACTOR *host = reinterpret_cast<iaIACTOR *>(ref.GetCbHost());
    RemoveMember(host);
}
