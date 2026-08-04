#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::FailBHVByNameAndStackAbove(const dsTSTRING<char>&) @ 0x8324C350
// ?FailBHVByNameAndStackAbove@aiBRAIN@@QAA_NABV?$dsTSTRING@D@@@Z
//
// Run the descriptor's "FailBHVByNameAndStackAbove" SSL function with a behaviour name; return its
// bool result.
bool aiBRAIN::FailBHVByNameAndStackAbove(const dsTSTRING<char> &bhvName)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<dsTSTRING<char> >(bhvName);
    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnFailBHVByNameAndStackAbove(spDesc.pointee), 1, &arg, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg and retVal destructors release their stored values at scope end.
}
