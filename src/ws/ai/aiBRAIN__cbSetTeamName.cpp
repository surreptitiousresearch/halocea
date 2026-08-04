#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"

// aiBRAIN::cbSetTeamName(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259210
// ?cbSetTeamName@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetTeamName" setter callback: unbox the team-name string out of argv[0] and forward it to the
// virtual implSetTeamName. `caller` self-releases on return.
void aiBRAIN::cbSetTeamName(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetTeamName(name.val);
    // name destructor self-releases at scope end.
}
