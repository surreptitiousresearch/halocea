#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::tSetAFCount(int) @ 0x832514C0
// ?tSetAFCount@aiBRAIN@@QAAXH@Z
//
// Run the descriptor's "tSetAFCount" SSL function with a single int arg (no return value used).
void aiBRAIN::tSetAFCount(int arg)
{
    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<int>(arg);

    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue(boxed);

    sslObject.CallFunc(aiBRAIN_DESC_SslFntSetAFCount(spDesc.pointee), 1, &argData, nullptr, sslOBJ_REF());
}
