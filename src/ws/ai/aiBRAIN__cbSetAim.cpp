#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetAim(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832567C0
// ?cbSetAim@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetAim" setter callback: unbox the aim-name string out of argv[0] and forward it to the
// virtual implSetAim. `caller` is passed by value and its ref self-releases on return.
void aiBRAIN::cbSetAim(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetAim(name.val);
}
