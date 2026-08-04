#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::tUnlock(int) @ 0x83251370
// ?tUnlock@aiBRAIN@@QAAXH@Z
//
// Run the descriptor's "tUnlock" SSL function with a single int arg (no return value used). The int
// is boxed into a dsDATA, then that dsDATA is copy-stored into the argv slot the way the binary does.
void aiBRAIN::tUnlock(int arg)
{
    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<int>(arg);

    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue(boxed); // dsDATA copy-store of the boxed int

    // RAII: the returned sslERROR temporary and the empty caller ref self-release on return.
    sslObject.CallFunc(aiBRAIN_DESC_SslFntUnlock(spDesc.pointee), 1, &argData, nullptr, sslOBJ_REF());
    // boxed / argData destructors release their stored values at scope end.
}
