#include "../../headers/ws/ai/aiWATCHER.h"
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiWAH_SYS.h"
#include "../../headers/ws/ai/aiWTC_HOLDER.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner

// aiWATCHER::Create @ 0x8328DBE8
// ?Create@aiWATCHER@@SAPAV1@PAViaIACTOR@@@Z
// Static factory: demand (find-or-create) the watcher holder for `actor` in the planner's watcher
// subsystem and return its held watcher (null when the holder could not be obtained).
aiWATCHER *aiWATCHER::Create(iaIACTOR *actor)
{
    aiWTC_HOLDER *holder = aiPlanner->wah.Demand(actor);
    if (!holder)
        return nullptr;
    return holder->watcher;
}
