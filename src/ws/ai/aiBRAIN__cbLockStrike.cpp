#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsDATA_SIMPLE_WRAPPER.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::cbLockStrike(int, dsDATA*, dsDATA&, sslOBJ_REF) @ 0x83256BD0
// ?cbLockStrike@aiBRAIN@@QAAXHPAVdsDATA@@AAV2@VsslOBJ_REF@@@Z
//
// SSL "LockStrike" setter callback: unbox the strike-name string (argv[0]) and a float lock time
// (argv[1]) and forward both to the virtual implLockStrike. `caller` self-releases on return.
void aiBRAIN::cbLockStrike(int /*argc*/, dsDATA *argv, dsDATA & /*retVal*/, sslOBJ_REF /*caller*/)
{
    float lockTime = 0.0f;
    argv[1].GetValue<float>(lockTime, DSD_CONV_RETRIEVE);
    dsDATA_SIMPLE_WRAPPER<dsTSTRING<char> > name(*argv, nullptr);
    implLockStrike(name.val, lockTime);
}
