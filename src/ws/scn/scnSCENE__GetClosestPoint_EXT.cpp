/* ?GetClosestPoint_EXT@scnSCENE@@QAAHPAUm3dV@@MPAVcdtREFINE@@PAVcdtINFO@@@Z @0x82744B58 */
#include "../../headers/ws/scn/scnSCENE.h"

// 0x82744B58 (scnSCENE::GetClosestPoint_EXT). Thin wrapper around GetClosestPoint_INT: if the
// caller passed an extended cdtINFO output block, clear it (9 consecutive dwords == 36 bytes ==
// sizeof(cdtINFO)) before running the query, then tail-call GetClosestPoint_INT with the same
// point/radius/refine/info arguments and return its result.
//
// DEVIATION: the DB's demangled prototype for this symbol is
//   int GetClosestPoint_EXT(m3dV*, float, cdtREFINE*, cdtINFO*)
// but the function body reads a fifth argument register (r7) and zero-fills the 36-byte block it
// points at, and the call sites supply it. Modeled here as the trailing `pInfoExt` output so the
// zeroing is represented faithfully; the demangled prototype simply undercounts this argument.
int scnSCENE::GetClosestPoint_EXT(m3dV *vTo, float radius, cdtREFINE *pRefine, cdtINFO *pInfo,
                                  cdtINFO *pInfoExt)
{
    if (pInfoExt)
    {
        // Zero the 36-byte extended-info output (matches the binary's 9-iteration stwu loop).
        int *dst = (int *)pInfoExt;
        for (int i = 0; i < 9; ++i)
            dst[i] = 0;
    }

    return GetClosestPoint_INT(vTo, radius, pRefine, pInfo);
}
