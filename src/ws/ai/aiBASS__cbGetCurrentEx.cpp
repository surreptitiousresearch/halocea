// aiBASS::cbGetCurrentEx @ 0x832417F8
// ?cbGetCurrentEx@aiBASS@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system instance

// SSL trampoline for GetCurrentEx(ai : object) : string. Resolve the callback host actor from
// argv[0] and box the name of the exclamation it is currently speaking into `retVal`.
void aiBASS::cbGetCurrentEx(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    sslOBJ_REF obj;
    obj.Set(argv[0], gsSslSystem);
    iaIACTOR *host = (iaIACTOR *)obj.GetCbHost();

    dsDATA result;
    result.type = nullptr;
    result.StoreValue<dsTSTRING<char> >(GetCurrentEx(host));
    retVal.StoreValue(result);
}
