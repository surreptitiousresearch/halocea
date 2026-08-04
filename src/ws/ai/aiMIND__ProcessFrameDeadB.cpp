#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiENEMY_BASE.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"

// aiMIND::ProcessFrameDeadB @ 0x832AA6B8
// ?ProcessFrameDeadB@aiMIND@@QAAXXZ
//
// Pre-body per-frame step for a dead unit: drop any lingering tracked enemy, clear the
// PERC_WAR flag, and — until the dead sequence has completed (AI_MIST_DEAD_COMPLETED, 0x400000)
// — keep running the best-behaviour pre-body update and asserting NEED_PROCESS_DEAD (0x200000).
void aiMIND::ProcessFrameDeadB()
{
    if (!enemy.pointee->Is(nullptr))
        ZeroEnemy();

    aiBRAIN *brain = static_cast<aiBRAIN *>(pBrain);
    brain->st.pointee->mind.val &= ~0x80u; // clear AI_MIST_PERC_WAR

    // deadNotDone: AI_MIST_DEAD_COMPLETED (0x400000) not yet set
    unsigned int deadNotDone = ((unsigned int)~brain->st.pointee->mind.val >> 22) & 1;
    if (deadNotDone)
        best.pointee->ProcessFrameBeforeBody();

    aiSTATUS *status = brain->st.pointee;
    if (deadNotDone)
        status->mind.val |= 0x200000;      // set AI_MIST_NEED_PROCESS_DEAD
    else
        status->mind.val &= ~0x200000u;    // clear AI_MIST_NEED_PROCESS_DEAD
}
