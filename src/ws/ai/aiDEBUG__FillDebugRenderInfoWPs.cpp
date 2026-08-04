#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/nav/navWP_ID_WEIGHT.h"

// aiDEBUG::FillDebugRenderInfoWPs @ 0x83197C20
//   (?FillDebugRenderInfoWPs@aiDEBUG@@SAXABV?$dsVECTOR@UnavWP_ID_WEIGHT@@$07@@PAFHAAH@Z)
//
// Static: copy the waypoint ids of the first min(from.nElem, debugInfoCapacity) weighted-waypoint
// entries into `debugInfo`, storing the copied count into `*count`. The per-element bounds assert
// in the decompiler output is the inlined dsVECTOR<navWP_ID_WEIGHT,8>::operator[].
void aiDEBUG::FillDebugRenderInfoWPs(const dsVECTOR<navWP_ID_WEIGHT, 8> &from, short *debugInfo,
                                     int debugInfoCapacity, int *count)
{
    int n = from.nElem;
    if (n >= debugInfoCapacity)
        n = debugInfoCapacity;
    *count = n;

    for (int i = 0; i < *count; ++i)
        debugInfo[i] = from[i].wpid;
}
