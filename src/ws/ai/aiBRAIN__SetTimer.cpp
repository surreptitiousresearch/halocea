#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::SetTimer(float, bool) @ 0x8324FA88
// ?SetTimer@aiBRAIN@@QAAHM_N@Z
//
// Run the descriptor's "SetTimer" SSL function with (duration, isCycle) and return the int timer id.
// The two arguments are marshalled into a contiguous dsDATA[2].
int aiBRAIN::SetTimer(float duration, bool isCycle)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<float>(duration);
    args[1].type = nullptr;
    args[1].StoreValue<bool>(isCycle);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetTimer(spDesc.pointee), 2, args, &retVal, sslOBJ_REF());

    int result = 0;
    retVal.GetValue<int>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
