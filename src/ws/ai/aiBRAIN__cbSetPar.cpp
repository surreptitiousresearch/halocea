#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetPar(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256928
// ?cbSetPar@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetPar" setter callback: read the float value out of argv[1] and the parameter-name string
// out of argv[0], then forward both to the virtual implSetPar(key, val). `caller` self-releases.
void aiBRAIN::cbSetPar(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float value = 0.0f;
    argv[1].GetValue<float>(value, DSD_CONV_RETRIEVE);

    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > key(*argv, nullptr);
    implSetPar(key.val, value);
}
