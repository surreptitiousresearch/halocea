#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::cbAvoidDanger(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259298
// ?cbAvoidDanger@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "AvoidDanger" callback: unbox the danger-name string out of argv[0], forward to implAvoidDanger,
// and box the returned bool into retVal. `caller` self-releases on return.
void aiBRAIN::cbAvoidDanger(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > danger(*argv, nullptr);
    bool result = implAvoidDanger(danger.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
    // out + danger destructors self-release at scope end.
}
