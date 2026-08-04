#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbOnExStarted(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832576F0
// ?cbOnExStarted@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "OnExStarted" callback: read the float (argv[1]) and unbox the exclamation-name string
// (argv[0]), then forward both to the virtual implOnExStarted(name, when). `caller` self-releases.
void aiBRAIN::cbOnExStarted(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float when = 0.0f;
    argv[1].GetValue<float>(when, DSD_CONV_RETRIEVE);

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implOnExStarted(name.val, when);
}
