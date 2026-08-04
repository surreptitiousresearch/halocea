#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::LockEnemy(float) @ 0x8324B418
// ?LockEnemy@aiBRAIN@@QAAXM@Z
//
// Run the descriptor's "LockEnemy" SSL function with a single float argument (no return value used).
void aiBRAIN::LockEnemy(float arg)
{
    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue<float>(arg);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFnLockEnemy(spDesc.pointee), 1, &argData, nullptr, sslOBJ_REF());
    // argData destructor releases its stored value at scope end.
}
