#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::tEnableForceAF(bool) @ 0x83251760
// ?tEnableForceAF@aiBRAIN@@QAAX_N@Z
//
// Run the descriptor's "tEnableForceAF" SSL function with a single bool arg (no return value used).
void aiBRAIN::tEnableForceAF(bool arg)
{
    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<bool>(arg);

    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue(boxed);

    sslObject.CallFunc(aiBRAIN_DESC_SslFntEnableForceAF(spDesc.pointee), 1, &argData, nullptr, sslOBJ_REF());
}
