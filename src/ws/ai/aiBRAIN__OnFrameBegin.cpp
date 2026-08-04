#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::OnFrameBegin() @ 0x83249438
// ?OnFrameBegin@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "OnFrameBegin" SSL event at the start of each frame (no arguments, no return value).
void aiBRAIN::OnFrameBegin()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnOnFrameBegin(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
