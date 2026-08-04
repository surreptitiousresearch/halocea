#include "../../headers/ws/gs/gsANITEC_ACTOR.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// 0x825B9880 — invoke the SSL "start cinematic" function on this actor's script object. The
// descriptor's cached sslFuncId_StartCine is called with no arguments and an empty caller ref;
// the returned sslERROR is discarded (its temporary's destructor releases the error data — the
// decompiler's manual refcount-decrement/dlFree on r6 is that sret sslERROR destructor).
void gsANITEC_ACTOR::StartCine()
{
    sslOBJ_REF caller; // default-constructed (empty) caller reference
    gsANITEC_ACTOR_DESC *desc = Desc<gsANITEC_ACTOR>(this);
    sslObject.CallFunc(desc->sslFuncId_StartCine, 0, nullptr, nullptr, caller);
}
