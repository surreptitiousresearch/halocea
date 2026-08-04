#include "../../headers/ws/ai/aiAPPROACH.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiAPPROACH::ProcessInZone() @ 0x8329AB38
// ?ProcessInZone@aiAPPROACH@@IAAXXZ
//
// Re-test the zone; if nobody is currently registered as in-zone, fire the brain's "approach leave"
// event with this module's name.
void aiAPPROACH::ProcessInZone()
{
    CheckDistance();
    if ((state.val & AI_APST_SOMEONE_IN_ZONE) == 0)
        pBrain->OnApproach(name, false);
}
