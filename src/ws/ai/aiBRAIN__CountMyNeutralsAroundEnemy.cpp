#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsCONVERTION_TYPE.h"

// aiBRAIN::CountMyNeutralsAroundEnemy(float) @ 0x83250B18
// ?CountMyNeutralsAroundEnemy@aiBRAIN@@QAAHM@Z
//
// Run the descriptor's "CountMyNeutralsAroundEnemy" SSL function with `radius` as its single
// argument and return the int result.
int aiBRAIN::CountMyNeutralsAroundEnemy(float radius)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<float>(radius);

    dsDATA retVal;
    retVal.type = nullptr;

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnCountMyNeutralsAroundEnemy(spDesc.pointee), 1, &arg, &retVal, sslOBJ_REF());

    int result = 0;
    retVal.GetValue<int>(result, DSD_CONV_RETRIEVE);
    return result;
    // arg / retVal destructors release their stored values at scope end.
}
