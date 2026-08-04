#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::SetSpreadAngle(float) @ 0x8324ACE8
// ?SetSpreadAngle@aiBRAIN@@QAAXM@Z
//
// Run the descriptor's "SetSpreadAngle" SSL function with an angle (no return value used).
void aiBRAIN::SetSpreadAngle(float angle)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<float>(angle);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetSpreadAngle(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
