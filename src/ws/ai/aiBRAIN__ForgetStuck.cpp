#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::ForgetStuck() @ 0x8324DDE0
// ?ForgetStuck@aiBRAIN@@QAAXXZ
//
// Run the descriptor's "ForgetStuck" SSL function (no arguments, no return value used).
void aiBRAIN::ForgetStuck()
{
    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnForgetStuck(spDesc.pointee), 0, nullptr, nullptr, sslOBJ_REF());
}
