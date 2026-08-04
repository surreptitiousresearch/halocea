#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::cbGetBool(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258B58
// ?cbGetBool@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetBool" getter callback: unbox the parameter-name string out of argv[0], forward it to the
// virtual implGetBool, and box the returned bool into retVal. `caller` self-releases on return.
void aiBRAIN::cbGetBool(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > desc(*argv, nullptr);
    bool result = implGetBool(desc.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<bool>(result);
    retVal.StoreValue(out);
    // out + desc destructors self-release at scope end.
}
