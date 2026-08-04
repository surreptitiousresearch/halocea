#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetApproach(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83257478
// ?cbSetApproach@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetApproach" setter callback: unbox the approach-name string out of argv[0] and forward it
// to the virtual implSetApproach. `caller` is passed by value and self-releases on return.
void aiBRAIN::cbSetApproach(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetApproach(name.val);
}
