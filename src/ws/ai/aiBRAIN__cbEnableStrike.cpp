#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbEnableStrike(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256AC0
// ?cbEnableStrike@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "EnableStrike" setter callback: unbox the strike-name string out of argv[0] and forward it to
// the virtual implEnableStrike. `caller` is passed by value and its ref self-releases on return.
void aiBRAIN::cbEnableStrike(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implEnableStrike(name.val);
}
