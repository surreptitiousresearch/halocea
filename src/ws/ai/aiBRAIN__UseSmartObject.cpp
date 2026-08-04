#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::UseSmartObject(const dsTSTRING<char>&, const dsTSTRING<char>&, const dsTSTRING<char>&, bool,
//                         const dsTSTRING<char>&) @ 0x8324E6D0
// ?UseSmartObject@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@00_N0@Z
//
// Run the descriptor's "UseSmartObject" SSL function with (object, action, anim, instant, exitAction),
// marshalled into a contiguous dsDATA[5]; returns its bool result.
bool aiBRAIN::UseSmartObject(const dsTSTRING<char> &object, const dsTSTRING<char> &action,
                            const dsTSTRING<char> &anim, bool instant, const dsTSTRING<char> &exitAction)
{
    dsDATA args[5];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(object);
    args[1].type = nullptr;
    args[1].StoreValue<dsTSTRING<char> >(action);
    args[2].type = nullptr;
    args[2].StoreValue<dsTSTRING<char> >(anim);
    args[3].type = nullptr;
    args[3].StoreValue<bool>(instant);
    args[4].type = nullptr;
    args[4].StoreValue<dsTSTRING<char> >(exitAction);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnUseSmartObject(spDesc.pointee), 5, args, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // args[] and retVal destructors release their stored values at scope end.
}
