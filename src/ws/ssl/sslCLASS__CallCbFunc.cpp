/* ?CallCbFunc@sslCLASS@@IAA?AVsslERROR@@PAVsslOBJECT@@HHPAVdsDATA@@AAV4@0@Z @0x82A398D0 */
#include "sslCLASS.h"
#include "sslCLASS_ELEMENT.h"
#include "sslCB_FUNCTION.h"
#include "sslCALL_FUNC_COUNTER.h"
#include "sslOBJECT.h"
#include "sslERROR.h"
#include "../ds/dsDATA.h"

// 0x82A398D0 — sslCLASS::CallCbFunc. Invoke the native-callback element `idx` on `pSelf`. If the
// object is still constructing (state bit 0x1 clear) the call is allowed only when the callback is
// flagged run-during-construct (flags bit 0x2 at sslFUNCTION_BASE offset 8); otherwise it is skipped
// with a no-error result. Returns sslERROR by value (sret): the decompiler swaps this/result
// (r3 = return slot sslERROR*, r4 = this sslCLASS*).
sslERROR sslCLASS::CallCbFunc(sslOBJECT *pSelf, int idx, int argc, dsDATA *argv, dsDATA &retVal, sslOBJECT *pCaller)
{
    sslCALL_FUNC_COUNTER counter(this->pSslSystem); // RAII call tracker on the owning system

    sslCB_FUNCTION *cb = this->elements[idx].pCbFunc;
    if ((pSelf->state.state & 1) == 0 || (cb->stateFunc & 2) != 0)
        return this->elements[idx].pCbFunc->Eval(pSelf, argc, argv, retVal, pCaller);

    return sslERROR();
}
