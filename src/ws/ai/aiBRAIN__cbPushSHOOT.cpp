#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbPushSHOOT(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256FE0
// ?cbPushSHOOT@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "PushSHOOT" setter callback: unbox the behaviour-name string out of argv[0], forward it to the
// virtual implPushSHOOT, and box the returned bool into retVal. `caller` self-releases on return.
void aiBRAIN::cbPushSHOOT(int /*argc*/, dsDATA *argv, dsDATA &retVal, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    bool pushed = implPushSHOOT(name.val);
    retVal.StoreValue<bool>(pushed);
}
