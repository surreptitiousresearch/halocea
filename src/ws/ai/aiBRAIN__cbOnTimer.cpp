#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbOnTimer(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324FED8
// ?cbOnTimer@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "OnTimer" callback: read the int timer id out of `argv[0]` and forward it to implOnTimer.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbOnTimer(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    int timerID = 0;
    argv->GetValue<int>(timerID, DSD_CONV_RETRIEVE);
    implOnTimer(timerID);
    // the caller ref releases itself at scope end.
}
