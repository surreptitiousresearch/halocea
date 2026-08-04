#include "../../headers/ws/ai/aiAPPROACH.h"
#include "../../headers/ws/ai/aiBRAIN.h"

// aiAPPROACH::ProcessFrame() @ 0x8329B098
// ?ProcessFrame@aiAPPROACH@@UAAXXZ  (vtbl slot 0x04)
//
// Per-frame update: while the module is enabled, re-test the trigger zone and, on a transition,
// fire the brain's OnApproach enter/leave event. `isEnter` is true when a watched entity has just
// entered the zone, false when the last one has just left.
void aiAPPROACH::ProcessFrame()
{
    unsigned int val = state.val;
    if ((val & AI_APST_APPROACH_ENABLED) == 0)
        return;

    bool isEnter;
    if (val & AI_APST_SOMEONE_IN_ZONE)
    {
        CheckDistance();
        if ((state.val & AI_APST_SOMEONE_IN_ZONE) != 0)
            return; // still occupied — no transition
        isEnter = false;
    }
    else
    {
        CheckDistance();
        if ((state.val & AI_APST_SOMEONE_IN_ZONE) == 0)
            return; // still empty — no transition
        isEnter = true;
    }

    pBrain->OnApproach(name, isEnter);
}
