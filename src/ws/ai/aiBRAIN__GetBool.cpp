#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::GetBool(const dsTSTRING<char>&) @ 0x8324F798
// ?GetBool@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z
//
// Run the species descriptor's "GetBool" SSL function with `name` as its single argument and return
// the bool result. The descriptor slot id is read out of spDesc.pointee (an aiBRAIN_DESC).
bool aiBRAIN::GetBool(const dsTSTRING<char> &name)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(name);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnGetBool(spDesc.pointee), 1, &arg, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg / retVal destructors release their stored values at scope end.
}
