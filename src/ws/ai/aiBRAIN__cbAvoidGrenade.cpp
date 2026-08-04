#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbAvoidGrenade(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259350
// ?cbAvoidGrenade@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "AvoidGrenade" callback: read an int from argv[0] and a float from argv[2], unbox the
// danger-name string out of argv[1], forward all three to implAvoidGrenade, and box the returned
// bool into retVal. `caller` self-releases on return.
void aiBRAIN::cbAvoidGrenade(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    float radius = 0.0f;
    argv[2].GetValue<float>(radius, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > danger(argv[1], nullptr);
    int grenadeId = 0;
    argv[0].GetValue<int>(grenadeId, DSD_CONV_RETRIEVE);

    bool result = implAvoidGrenade(grenadeId, danger.val, radius);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
    // out + danger destructors self-release at scope end.
}
