#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"

// aiBRAIN::cbSetFloat(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83258AD0
// ?cbSetFloat@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetFloat" callback: unbox (name, value) out of argv and forward to the virtual implSetFloat.
// No return value. `caller` self-releases on return.
void aiBRAIN::cbSetFloat(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float value = 0.0f;
    argv[1].GetValue<float>(value, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(argv[0], nullptr);

    implSetFloat(name.val, value);
}
