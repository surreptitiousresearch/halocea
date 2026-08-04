#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::SetForceSpeed(const dsTSTRING<char>&) @ 0x8324E4D8
// ?SetForceSpeed@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "SetForceSpeed" SSL function with a speed-preset name; returns its bool result.
bool aiBRAIN::SetForceSpeed(const dsTSTRING<char> &speed)
{
    dsDATA args[1];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(speed);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetForceSpeed(spDesc.pointee), 1, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
