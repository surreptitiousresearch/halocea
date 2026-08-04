#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::QOT_Reset() @ 0x832518B0
// ?QOT_Reset@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "QOT_Reset" SSL function (no args, no return value used).
void aiBRAIN::QOT_Reset()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnQOT_Reset(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
