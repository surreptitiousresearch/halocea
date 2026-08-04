#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbSetSpreadAngle(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324ADD0
// ?cbSetSpreadAngle@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetSpreadAngle" setter callback: read a float out of argv[0] and forward it to the body via
// implSetSpreadAngle. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbSetSpreadAngle(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float angle = 0.0f;
    argv->GetValue<float>(angle, DSD_CONV_RETRIEVE);
    implSetSpreadAngle(angle);
    // the caller ref releases itself at scope end.
}
