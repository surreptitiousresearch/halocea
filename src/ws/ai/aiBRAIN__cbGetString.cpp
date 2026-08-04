#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::cbGetString(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258E60
// ?cbGetString@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetString" getter callback: unbox the parameter-name string out of argv[0], forward it to the
// virtual implGetString, and box the returned string into retVal. `caller` self-releases on return.
void aiBRAIN::cbGetString(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > desc(*argv, nullptr);
    dsTSTRING<char> result = implGetString(desc.val);

    dsDATA out;
    out.type = nullptr;
    out.StoreValue<dsTSTRING<char> >(result);
    retVal.StoreValue(out);
    // out + result + desc destructors self-release at scope end.
}
