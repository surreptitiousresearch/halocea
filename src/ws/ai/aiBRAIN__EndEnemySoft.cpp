#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::EndEnemySoft() @ 0x832498C8
// ?EndEnemySoft@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "EndEnemySoft" SSL event to softly release the current enemy (no arguments, no return value).
void aiBRAIN::EndEnemySoft()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnEndEnemySoft(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
