#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::PushSHOOT(const dsTSTRING<char>&) @ 0x8324BCE8
// ?PushSHOOT@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "PushSHOOT" SSL function with a behaviour name and return its bool result. The
// name is copied into a local dsTSTRING (ref-counted), marshalled into a dsDATA, then dispatched.
bool aiBRAIN::PushSHOOT(const dsTSTRING<char> &bhvName)
{
    dsTSTRING<char> name = bhvName;

    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue<dsTSTRING<char> >(name);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnPushSHOOT(spDesc.pointee), 1, &argData, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // argData, retVal and name destructors release their stored values / buffer refs at scope end.
}
