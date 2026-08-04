#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbGetFloat(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258A18
// ?cbGetFloat@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "GetFloat" callback: unbox the param name out of argv[0] and forward to the virtual
// implGetFloat; store its float result into `retVal`. `caller` self-releases on return.
void aiBRAIN::cbGetFloat(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(argv[0], nullptr);

    float value = implGetFloat(name.val);
    retVal.StoreValue<float>(value);
}
