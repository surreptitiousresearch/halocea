#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::UseSmartObjectInstant(const dsTSTRING<char>&, const dsTSTRING<char>&, bool) @ 0x8324EA60
// ?UseSmartObjectInstant@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@0_N@Z
//
// Run the descriptor's "UseSmartObjectInstant" SSL function with (object, action, keepPlaying),
// marshalled into a contiguous dsDATA[3]; returns its bool result.
bool aiBRAIN::UseSmartObjectInstant(const dsTSTRING<char> &object, const dsTSTRING<char> &action, bool keepPlaying)
{
    dsDATA args[3];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(object);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(action);
    args[2].type = nullptr;
    args[2].StoreValue<bool>(keepPlaying);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnUseSmartObjectInstant(spDesc.pointee), 3, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
