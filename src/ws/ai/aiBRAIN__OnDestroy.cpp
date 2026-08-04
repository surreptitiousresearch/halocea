#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::OnDestroy() @ 0x832493B8
// ?OnDestroy@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "OnDestroy" SSL event when the brain is destroyed (no arguments, no return value).
void aiBRAIN::OnDestroy()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnOnDestroy(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
