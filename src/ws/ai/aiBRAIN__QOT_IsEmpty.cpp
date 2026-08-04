#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::QOT_IsEmpty() @ 0x83251B00
// ?QOT_IsEmpty@aiBRAIN@@QAA_NXZ
//
// Run the descriptor's "QOT_IsEmpty" SSL function and return its bool result.
bool aiBRAIN::QOT_IsEmpty()
{
    dsDATA retVal;
    retVal.type = nullptr;

    sslObject.CallFunc(aiBRAIN_DESC_SslFnQOT_IsEmpty(spDesc.pointee), 0, nullptr, &retVal, sslOBJ_REF());

    bool empty = false;
    retVal.GetValue<bool>(empty, DSD_CONV_RETRIEVE);
    return empty;
}
