#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::cbtWaitBHV(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259438
// ?cbtWaitBHV@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "tWaitBHV" (timer wait-behaviour) setter callback: unbox the behaviour-name string out of
// argv[0] and forward it to the virtual impltWaitBHV. `caller` self-releases on return.
void aiBRAIN::cbtWaitBHV(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > bhv(*argv, nullptr);
    impltWaitBHV(bhv.val);
    // bhv destructor self-releases at scope end.
}
