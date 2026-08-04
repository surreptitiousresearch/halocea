#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::EnemyGetMovementDist() @ 0x8324B890
// ?EnemyGetMovementDist@aiBRAIN@@QAAMXZ
//
// Run the descriptor's "EnemyGetMovementDist" SSL function (no arguments) and return its float result.
float aiBRAIN::EnemyGetMovementDist()
{
    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnEnemyGetMovementDist(spDesc.pointee), 0, nullptr, &retVal, sslOBJ_REF());

    float result = 0.0f;
    retVal.GetValue<float>(result, DSD_CONV_RETRIEVE);
    return result;
    // retVal destructor releases its stored value at scope end.
}
