#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::EndEnemy() @ 0x83249848
// ?EndEnemy@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "EndEnemy" SSL event to stop tracking the current enemy (no arguments, no return value).
void aiBRAIN::EndEnemy()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnEndEnemy(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
