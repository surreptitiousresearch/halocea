#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbCallBhvFuncOnce(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83257258
// ?cbCallBhvFuncOnce@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "CallBhvFuncOnce" callback: unbox the behaviour name (argv[0]) and function name (argv[1])
// and forward both to the virtual implCallBhvFuncOnce. `caller` self-releases on return.
void aiBRAIN::cbCallBhvFuncOnce(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > funcName(argv[1], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > bhvName(argv[0], nullptr);
    implCallBhvFuncOnce(bhvName.val, funcName.val);
}
