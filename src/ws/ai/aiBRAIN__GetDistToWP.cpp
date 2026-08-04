#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::GetDistToWP(const dsTSTRING<char>&) @ 0x8324E0A8
// ?GetDistToWP@aiBRAIN@@QAAMABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "GetDistToWP" SSL function with waypoint name `wp` as its single argument,
// and return the float distance result.
float aiBRAIN::GetDistToWP(const dsTSTRING<char> &wp)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(wp);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetDistToWP(spDesc.pointee), 1, &arg, &retVal, sslOBJ_REF());

    float result = 0.0f;
    retVal.GetValue<float>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg and retVal destructors release their stored values at scope end.
}
