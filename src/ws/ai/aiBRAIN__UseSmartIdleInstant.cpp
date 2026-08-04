#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::UseSmartIdleInstant(const dsTSTRING<char>&, const dsTSTRING<char>&) @ 0x8324F268
// ?UseSmartIdleInstant@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0@Z
//
// Run the descriptor's "UseSmartIdleInstant" SSL function with (object, action), marshalled into a
// contiguous dsDATA[2]; returns its bool result.
bool aiBRAIN::UseSmartIdleInstant(const dsTSTRING<char> &object, const dsTSTRING<char> &action)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(object);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(action);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnUseSmartIdleInstant(spDesc.pointee), 2, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
