#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::QOT_Size() @ 0x832519B0
// ?QOT_Size@aiBRAIN@@QAAHXZ
//
// Run the descriptor's "QOT_Size" SSL function and return its int result (the queue size). The
// result is marshalled back through a dsDATA retVal and extracted with GetValue<int>.
int aiBRAIN::QOT_Size()
{
    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnQOT_Size(spDesc.pointee), 0, nullptr, &retVal, sslOBJ_REF());

    int size = 0;
    retVal.GetValue<int>(size, DSD_CONV_RETRIEVE);
    return size;
    // retVal destructor releases its stored value at scope end.
}
