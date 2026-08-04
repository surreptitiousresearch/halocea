#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBRAIN_DESC_sslfunc_boundary.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/ssl/sslOBJ_REF.h"
#include "../../headers/ws/ssl/sslERROR.h"
#include "../../headers/ws/ds/dsDATA.h"

// aiBRAIN::tSetAFDelay(float) @ 0x83251610
// ?tSetAFDelay@aiBRAIN@@QAAXM@Z
//
// Run the descriptor's "tSetAFDelay" SSL function with a single float arg (no return value used).
void aiBRAIN::tSetAFDelay(float arg)
{
    dsDATA boxed;
    boxed.type = nullptr;
    boxed.StoreValue<float>(arg);

    dsDATA argData;
    argData.type = nullptr;
    argData.StoreValue(boxed);

    sslObject.CallFunc(aiBRAIN_DESC_SslFntSetAFDelay(spDesc.pointee), 1, &argData, nullptr, sslOBJ_REF());
}
