#include "../../headers/ws/ai/aiPERC.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiSTATUS.h"
#include "../../headers/ws/ent/entENTITY.h"
#include "../../headers/ws/ds/dsCMP.h"
#include "../../headers/ws/dsc/dscDESC.h"
#include "../../headers/ws/fsm/fsmFLOAT_ARRAY.h"

// aiPERC::NoticeGrenade @ 0x8329D468
// ?NoticeGrenade@aiPERC@@UAAXPAVentENTITY@@@Z
//
// Queue `entGrenade` for grenade avoidance if the unit is alive (die flags low 2 bits clear), has
// grenade-avoidance enabled (gen bit 15), no grenade already queued, and this grenade is not
// already in the status's tracked-id list. Arms the notify countdown from descriptor cell 49.
void aiPERC::NoticeGrenade(entENTITY *entGrenade)
{
    if (grenadeMPID != -1)
        return;

    aiSTATUS *st = pBrain->st.pointee;
    if ((st->die.val & 3) != 0)
        return;
    if (((static_cast<unsigned int>(st->gen.val) >> 15) & 1) == 0)
        return;

    int uid = entGrenade->uniqueID;
    dsCMP cmp = {};
    if (st->perc.grenadeMPIDs.Find<dsCMP, int>(uid, cmp, 0) == -1) {
        grenadeMPID = uid;
        timerNotifyGrenade =
            reinterpret_cast<const fsmFLOAT_ARRAY *>(&pBrain->spDesc.pointee[49].pBrand)->GetValue();
    }
}
