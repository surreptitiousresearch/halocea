#include "../../headers/ws/ai/aiSQUAD.h"
#include "../../headers/ws/ai/aiSQUAD_DESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ssl/sslSYSTEM.h"
#include "../../headers/ws/ds/dsDATA.h"

extern sslSYSTEM *gsSslSystem; // ?gsSslSystem@@3PAVsslSYSTEM@@A — global SSL system instance

// aiSQUAD::GetGroup() @ 0x832A8730  (?GetGroup@aiSQUAD@@QAA?AVsslOBJ_REF@@XZ)
// Run the descriptor's "GetGroup" SSL function (no args) and convert its type-erased result into an
// sslOBJ_REF (via Set), returned by value.
sslOBJ_REF aiSQUAD::GetGroup()
{
    aiSQUAD_DESC *desc = reinterpret_cast<aiSQUAD_DESC *>(spDesc.pointee);

    dsDATA retData;
    retData.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(desc->sslFuncId_GetGroup, 0, nullptr, &retData, sslOBJ_REF());

    sslOBJ_REF group;
    group.Set(retData, gsSslSystem);
    return group;
    // retData's destructor releases its stored value at scope end.
}
