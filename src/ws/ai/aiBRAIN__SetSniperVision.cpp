#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::SetSniperVision(bool) @ 0x8324A1B8
// ?SetSniperVision@aiBRAIN@@QAAX_N@Z
//
// Run the descriptor's "SetSniperVision" SSL function with an on/off flag (no return value used).
void aiBRAIN::SetSniperVision(bool enable)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<bool>(enable);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnSetSniperVision(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
