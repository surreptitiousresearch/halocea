#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbLockEnemy(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x8324B500
// ?cbLockEnemy@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "LockEnemy" setter callback: read a float out of `argv[0]` and forward it to implLockEnemy.
// `caller` is passed by value and its ref is released on return.
void aiBRAIN::cbLockEnemy(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float arg = 0.0f;
    argv->GetValue<float>(arg, DSD_CONV_RETRIEVE);
    implLockEnemy(arg);
    // the caller ref releases itself at scope end.
}
