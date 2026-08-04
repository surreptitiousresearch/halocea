#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbSetSniperVision(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324A2A0
// ?cbSetSniperVision@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "SetSniperVision" setter callback: read a bool out of `argv[0]` and forward it to the body via
// implSetSniperVision. `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbSetSniperVision(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    bool enable = false;
    argv->GetValue<bool>(enable, DSD_CONV_RETRIEVE);
    implSetSniperVision(enable);
    // the caller ref releases itself at scope end.
}
