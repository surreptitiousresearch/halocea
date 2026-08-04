#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiEXIFACE_boundaries.h" // nedNAV_EDITOR::AddToUnused

// aiPLANNER::NoticeNavEdUnusedWP @ 0x83197178
//   (?NoticeNavEdUnusedWP@aiPLANNER@@UAAXABV?$dsVECTOR@F$07@@@Z)
//
// Virtual: forward every waypoint id in `wps` to the nav editor's "unused" set. The per-element
// bounds assert in the decompiler output is the inlined dsVECTOR<short,8>::operator[]; it is kept
// implicit here by indexing through operator[].
void aiPLANNER::NoticeNavEdUnusedWP(const dsVECTOR<short, 8> &wps)
{
    for (int i = 0; i < wps.nElem; ++i)
        this->navEditor->AddToUnused(wps[i]);
}
