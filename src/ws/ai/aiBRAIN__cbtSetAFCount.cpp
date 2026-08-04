#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbtSetAFCount(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x832515A8
// ?cbtSetAFCount@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "tSetAFCount" setter callback: read an int out of argv[0] and forward it to impltSetAFCount.
void aiBRAIN::cbtSetAFCount(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    int value = 0;
    argv->GetValue<int>(value, DSD_CONV_RETRIEVE);
    impltSetAFCount(value);
}
