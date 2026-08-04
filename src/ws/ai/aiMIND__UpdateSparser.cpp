#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner

// aiMIND::UpdateSparser @ 0x83229658
// ?UpdateSparser@aiMIND@@QAAXXZ
//
// Maintain the "run mind frame" flag (aiSTATUS::mind bit 0) each tick. Dead brains never run a
// mind frame; the global AI-off-mind debug toggle forces it on; otherwise the LOD scheduler
// decides, and when the frame runs we refresh how long default-CDT requests are kept alive.
void aiMIND::UpdateSparser()
{
    aiBRAIN  *brain = static_cast<aiBRAIN *>(pBrain);
    aiSTATUS *st    = brain->st.pointee;

    if (brain->IsDead())
    {
        st->mind.val &= ~1;
    }
    else if (aiDEBUG::IsOffMind())
    {
        st->mind.val |= 1;
    }
    else
    {
        bool skip = aiPlanner->LOD.ShouldSkipMindFrame(brain) != 0;
        if (skip) st->mind.val |= 1; else st->mind.val &= ~1;
        st->lod.timeKeepRequests = CalcKeepDefCDTReqTime();
    }
}
