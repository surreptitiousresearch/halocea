// rendLGT_MNG__GetStaticLight @0x8275AC38
#include "../../headers/ws/rend/rendLGT_MNG.h"
#include "../../headers/ws/ds/dsVECTOR.h"

// rendLGT_MNG::GetStaticLight @ 0x8275AC4C
//
// Fetch the ref-counted static light smart-pointer at slot `i`. An out-of-range index logs an
// error (but still indexes, matching the shipped behaviour — the operator[] is not guarded).

// boundary: ws logging helper (?_apLog / variadic).
extern void _apLog(const char *fmt, ...);

const rendLIGHT_SP *rendLGT_MNG::GetStaticLight(int i)
{
    int nElem = staticLights.nElem;
    if (i >= nElem || i < 0)
        _apLog("~Lights,Error~Invalid access to static lights, id = %i, arr length = %i", i, nElem);
    return &staticLights[i];
}
