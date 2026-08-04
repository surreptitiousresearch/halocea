#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"
#include "../../headers/ws/ds/dsTSTRING.h"

// aiBRAIN::LockStrike(const dsTSTRING<char>&, float) @ 0x8324A848
// ?LockStrike@aiBRAIN@@QAAXABV?$dsTSTRING@D@@M@Z
//
// Run the descriptor's "LockStrike" SSL function with (strike name, lock time). The two arguments
// are marshalled into a contiguous dsDATA[2] array (no return value used).
void aiBRAIN::LockStrike(const dsTSTRING<char> &strike, float lockTime)
{
    dsDATA args[2];
    args[0].type = nullptr;
    args[0].StoreValue<dsTSTRING<char> >(strike);
    args[1].type = nullptr;
    args[1].StoreValue<float>(lockTime);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnLockStrike(spDesc.pointee), 2, args, nullptr, sslOBJ_REF());
    // both args' destructors release their stored values at scope end.
}
