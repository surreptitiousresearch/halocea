#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::OnDbgOut() @ 0x832494B8
// ?OnDbgOut@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "OnDbgOut" SSL event for debug output (no arguments, no return value).
void aiBRAIN::OnDbgOut()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnOnDbgOut(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
