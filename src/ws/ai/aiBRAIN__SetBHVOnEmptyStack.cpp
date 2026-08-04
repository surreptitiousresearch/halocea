#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::SetBHVOnEmptyStack() @ 0x8324C688
// ?SetBHVOnEmptyStack@aiBRAIN@@QAA_NXZ
//
// Run the descriptor's "SetBHVOnEmptyStack" SSL function (no arguments) and return its bool result.
bool aiBRAIN::SetBHVOnEmptyStack()
{
    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetBHVOnEmptyStack(spDesc.pointee), 0, nullptr, &retVal, sslOBJ_REF());

    bool result = false;
    retVal.GetValue<bool>(result, DSD_CONV_RETRIEVE);
    return result;
    // retVal destructor releases its stored value at scope end.
}
