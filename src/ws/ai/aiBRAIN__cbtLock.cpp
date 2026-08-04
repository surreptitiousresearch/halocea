#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbtLock(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83251308
// ?cbtLock@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "tLock" setter callback: read an int out of `argv[0]` and forward it to impltLock.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbtLock(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    int arg = 0;
    argv->GetValue<int>(arg, DSD_CONV_RETRIEVE);
    impltLock(arg);
    // the caller ref releases itself at scope end.
}
