#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::cbGetInt(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258C10
// ?cbGetInt@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetInt" getter callback: unbox the parameter-name string out of argv[0], forward it to the
// virtual implGetInt, and box the returned int into retVal. `caller` self-releases on return.
void aiBRAIN::cbGetInt(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > desc(*argv, nullptr);
    int result = implGetInt(desc.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<int>(result);
    retVal.StoreValue(out);
    // out + desc destructors self-release at scope end.
}
