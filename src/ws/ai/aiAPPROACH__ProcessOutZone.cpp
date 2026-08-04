#include "../../headers/ws/ai/aiAPPROACH.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiAPPROACH::ProcessOutZone() @ 0x8329AB88
// ?ProcessOutZone@aiAPPROACH@@IAAXXZ
//
// Re-test the zone; if somebody is now registered as in-zone, fire the brain's "approach enter"
// event with this module's name.
void aiAPPROACH::ProcessOutZone()
{
    CheckDistance();
    if ((state.val & AI_APST_SOMEONE_IN_ZONE) != 0)
        pBrain->OnApproach(name, true);
}
