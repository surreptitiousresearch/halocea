#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ai/aiMIND.h"
#include "../../headers/ws/ai/aiBEST_BASE.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ds/dsCMP.h"

// aiPERC::NoticeGrenadeTerm @ 0x8329E330
// ?NoticeGrenadeTerm@aiPERC@@UAAXPAVentENTITY@@@Z
//
// A tracked grenade ended. If it was the one queued for avoidance, drop the queue slot; otherwise
// remove it from the status's tracked-id list and relay the termination to the best-behaviour
// module.
void aiPERC::NoticeGrenadeTerm(entENTITY *entGrenade)
{
    if (timerNotifyGrenade > 0.000001f && grenadeMPID == entGrenade->uniqueID) {
        grenadeMPID = -1;
        timerNotifyGrenade = -1.0f;
    } else {
        int uid = entGrenade->uniqueID;
        dsVECTOR<int, 8> &ids = pBrain->st.pointee->perc.grenadeMPIDs;
        dsCMP cmp = {};
        int idx = ids.Find<dsCMP, int>(uid, cmp, 0);
        if (idx != -1)
            ids.Erase(idx, 1);
        pBrain->mind.best.pointee->NoticeGrenadeTerm(entGrenade);
    }
}
