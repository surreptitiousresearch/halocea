#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiGRASS.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner
#include "../../headers/ws/ent/entENTITY.h"

// aiPERC::UpdateGrenadeAvoidance @ 0x8329CC38
// ?UpdateGrenadeAvoidance@aiPERC@@IAAXXZ
//
// When the notify countdown reaches zero, hand the queued grenade off to the best-behaviour module
// so it can start avoiding it, then clear the queue slot.
void aiPERC::UpdateGrenadeAvoidance()
{
    if (timerNotifyGrenade >= -0.000001f && timerNotifyGrenade <= 0.000001f) {
        int grenadeId = grenadeMPID;
        grenadeMPID = -1;
        timerNotifyGrenade = -1.0f;
        if (aiPlanner) {
            const aiGRASS::GRENADE_INFO *info =
                (&aiPlanner->grass)->GetGrenadeInfo(grenadeId);
            if (info)
                pBrain->mind.best.pointee->NoticeGrenade(const_cast<entENTITY *>(info->grenade));
        }
    }
}
