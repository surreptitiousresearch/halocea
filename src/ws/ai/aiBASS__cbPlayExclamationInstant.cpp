// aiBASS::cbPlayExclamationInstant @ 0x83242398
// ?cbPlayExclamationInstant@aiBASS@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system instance

// SSL trampoline for PlayExclamationInstant(ai : object, sound : string) : bool.
void aiBASS::cbPlayExclamationInstant(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > sound(argv[1], nullptr);

    sslOBJ_REF obj;
    obj.Set(argv[0], gsSslSystem);
    iaIACTOR *host = (iaIACTOR *)obj.GetCbHost();

    bool res = PlayExclamationInstant(host, sound.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
}
