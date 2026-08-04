#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbDbgStrike(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256C58
// ?cbDbgStrike@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "DbgStrike" setter callback: unbox the strike-name string out of argv[0] and forward it to the
// virtual implDbgStrike. `caller` is passed by value and its ref self-releases on return.
void aiBRAIN::cbDbgStrike(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implDbgStrike(name.val);
}
