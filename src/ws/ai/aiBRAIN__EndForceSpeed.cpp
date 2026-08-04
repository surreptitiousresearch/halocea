#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::EndForceSpeed() @ 0x8324E650
// ?EndForceSpeed@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "EndForceSpeed" SSL function (no args, no return value).
void aiBRAIN::EndForceSpeed()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnEndForceSpeed(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
