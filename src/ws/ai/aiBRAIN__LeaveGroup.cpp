#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"

// aiBRAIN::LeaveGroup() @ 0x83250150
// ?LeaveGroup@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "LeaveGroup" SSL function (no arguments, no return value).
void aiBRAIN::LeaveGroup()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnLeaveGroup(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
