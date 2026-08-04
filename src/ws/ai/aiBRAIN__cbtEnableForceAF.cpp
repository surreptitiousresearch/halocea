#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbtEnableForceAF(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83251848
// ?cbtEnableForceAF@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "tEnableForceAF" setter callback: read a bool out of argv[0] and forward it to
// impltEnableForceAF.
void aiBRAIN::cbtEnableForceAF(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    bool value = false;
    argv->GetValue<bool>(value, DSD_CONV_RETRIEVE);
    impltEnableForceAF(value);
}
