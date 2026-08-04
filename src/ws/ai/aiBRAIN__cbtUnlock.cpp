#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbtUnlock(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83251458
// ?cbtUnlock@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "tUnlock" setter callback: read an int out of argv[0] and forward it to impltUnlock. 
// is passed by value and its ref is released on return.
void aiBRAIN::cbtUnlock(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    int value = 0;
    argv->GetValue<int>(value, DSD_CONV_RETRIEVE);
    impltUnlock(value);
    // the caller ref releases itself at scope end.
}
