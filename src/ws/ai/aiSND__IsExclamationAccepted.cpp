#include "../../headers/ws/ai/aiSND.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiBASS.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner

// aiSND::IsExclamationAccepted @ 0x832993B8
// ?IsExclamationAccepted@aiSND@@UAA_NABVdsSTRID@@@Z
//
// Whether exclamation `name` may play right now. Exclamations must be permitted at all; a live brain
// then always accepts, while a dead one defers to the planner's BASS accept-for-dead policy.
bool aiSND::IsExclamationAccepted(const dsSTRID &name)
{
    if ((permissions.val & AI_SNDP_ENABLE_EXCLAMATIONS) == 0)
        return false;

    if (pBrain->IsDead())
        return (&aiPlanner->bass)->IsExAcceptForDead(name);

    return true;
}
