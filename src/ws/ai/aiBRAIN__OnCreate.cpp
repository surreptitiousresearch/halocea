#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::OnCreate() @ 0x83249338
// ?OnCreate@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "OnCreate" SSL event when the brain is created (no arguments, no return value).
void aiBRAIN::OnCreate()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnOnCreate(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
