#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::ForceWar() @ 0x8324A308
// ?ForceWar@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "ForceWar" SSL function (no arguments, no return value).
void aiBRAIN::ForceWar()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnForceWar(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
