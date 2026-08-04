#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::OnTimer(int) @ 0x8324FDF0
// ?OnTimer@aiBRAIN@@QAAXH@Z
//
// Run the descriptor's "OnTimer" SSL function with the timer id as its single argument (no return).
void aiBRAIN::OnTimer(int timerID)
{
    dsDATA arg;
    arg.type = nullptr;
    arg.StoreValue<int>(timerID);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnOnTimer(spDesc.pointee), 1, &arg, nullptr, sslOBJ_REF());
    // arg destructor releases its stored value at scope end.
}
