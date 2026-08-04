#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::GetFloat(const dsTSTRING<char>&) @ 0x8324F478
// ?GetFloat@aiBRAIN@@QAAMABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "GetFloat" SSL function with a param name; returns the float result.
float aiBRAIN::GetFloat(const dsTSTRING<char> &name)
{
    dsDATA args[1];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(name);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetFloat(spDesc.pointee), 1, args, &retVal, sslOBJ_REF());

    float result = 0.0f;
    retVal.GetValue<float>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
