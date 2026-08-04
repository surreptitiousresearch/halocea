#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::SetContourShootingDist(float, float, float, float) @ 0x8324B048
// ?SetContourShootingDist@aiBRAIN@@QAAXMMMM@Z
//
// Run the descriptor's "SetContourShootingDist" SSL function with four distance bounds. The four
// arguments are marshalled into a contiguous dsDATA[4] array (no return value used).
void aiBRAIN::SetContourShootingDist(float d0, float d1, float d2, float d3)
{
    dsDATA args[4];
    args[0].type = nullptr;
    args[0].StoreValue<float>(d0);
    args[1].type = nullptr;
    args[1].StoreValue<float>(d1);
    args[2].type = nullptr;
    args[2].StoreValue<float>(d2);
    args[3].type = nullptr;
    args[3].StoreValue<float>(d3);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetContourShootingDist(spDesc.pointee), 4, args, nullptr, sslOBJ_REF());
    // all args' destructors release their stored values at scope end.
}
