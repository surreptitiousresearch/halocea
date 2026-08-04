#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::KillTimer(int) @ 0x8324FCA0
// ?KillTimer@aiBRAIN@@QAAXH@Z
//
// Run the descriptor's "KillTimer" SSL function with the timer id as its single argument (no return).
void aiBRAIN::KillTimer(int timerID)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<int>(timerID);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnKillTimer(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
