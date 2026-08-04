#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetWpn(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256CE0
// ?cbSetWpn@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetWpn" setter callback: unbox the weapon-name string out of argv[0] and forward it to the
// virtual implSetWpn. `caller` is passed by value and its ref self-releases on return.
void aiBRAIN::cbSetWpn(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implSetWpn(name.val);
}
