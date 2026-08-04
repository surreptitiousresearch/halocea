// aiBASS::cbPlayExclamation @ 0x832424B0
// ?cbPlayExclamation@aiBASS@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ia/iaIACTOR.h"

extern sslSYSTEM *gsSslSystem; // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system instance

// SSL trampoline for
//   PlayExclamation(ai : object, sound : string, priority : float = -1, overlap : string = "") : bool.
void aiBASS::cbPlayExclamation(int argc, dsDATA *argv, dsDATA &retVal, sslOBJ_REF caller)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > overlap(argv[3], nullptr);
    float priority;
    argv[2].GetValue<float>(priority, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > sound(argv[1], nullptr);

    sslOBJ_REF obj;
    obj.Set(argv[0], gsSslSystem);
    iaIACTOR *host = (iaIACTOR *)obj.GetCbHost();

    bool res = PlayExclamation(host, sound.val, priority, overlap.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(res);
    retVal.StoreValue(out);
}
