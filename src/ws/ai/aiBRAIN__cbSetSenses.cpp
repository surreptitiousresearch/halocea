#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetSenses(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832569B0
// ?cbSetSenses@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetSenses" setter callback: unbox the senses-name string out of argv[0] and forward it to
// the virtual implSetSenses. `caller` is passed by value and its ref self-releases on return.
void aiBRAIN::cbSetSenses(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetSenses(name.val);
}
