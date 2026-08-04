#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/ai/aiCOORDINATOR.h"
#include "../../headers/ws/ai/aiCOORD_GROUP_boundary.h"

// aiPERC::UpdateWarPeace @ 0x8329D2C8
// ?UpdateWarPeace@aiPERC@@IAAXXZ
//
// Recompute the mind "battle" flag (mind bit 0x80) each frame via the group- or single-selection
// path, and fire the SSL OnBattleChanged event when the flag toggles.
void aiPERC::UpdateWarPeace()
{
    aiSTATUS *st = pBrain->st.pointee;
    unsigned int before = (static_cast<unsigned int>(st->mind.val) >> 7) & 1;

    aiCOORD_GROUP *grp =
        (&aiPlanner->coordinator)->GetGroup_2(pBrain);
    if (grp)
        ApplyGroupSelectionWarPeace(*grp);
    else
        ApplySingleSelectionWarPeace();

    if (before != ((static_cast<unsigned int>(st->mind.val) >> 7) & 1))
        pBrain->mind.CallSSL_OnBattleChanged();
}
