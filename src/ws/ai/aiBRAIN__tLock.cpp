#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::tLock(int) @ 0x83251220
// ?tLock@aiBRAIN@@QAAXH@Z
//
// Run the descriptor's "tLock" SSL function with a single int argument (no return value used).
void aiBRAIN::tLock(int arg)
{
    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue<int>(arg);

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFntLock(spDesc.pointee), 1, &argData, nullptr, sslOBJ_REF());
    // argData destructor releases its stored value at scope end.
}
