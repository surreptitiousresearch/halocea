#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbSetInterest(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83259068
// ?cbSetInterest@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetInterest" callback: read a bool from argv[2] and unbox two strings (argv[0], argv[1]),
// then forward all three to implSetInterest. `caller` self-releases on return.
void aiBRAIN::cbSetInterest(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    bool on = false;
    argv[2].GetValue<bool>(on, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(argv[0], nullptr);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > kind(argv[1], nullptr);
    implSetInterest(name.val, kind.val, on);
    // wrapper destructors self-release at scope end.
}
