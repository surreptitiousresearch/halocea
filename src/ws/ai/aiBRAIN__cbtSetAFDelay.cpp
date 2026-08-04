#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbtSetAFDelay(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832516F8
// ?cbtSetAFDelay@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "tSetAFDelay" setter callback: read a float out of argv[0] and forward it to impltSetAFDelay.
void aiBRAIN::cbtSetAFDelay(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float value = 0.0f;
    argv->GetValue<float>(value, DSD_CONV_RETRIEVE);
    impltSetAFDelay(value);
}
