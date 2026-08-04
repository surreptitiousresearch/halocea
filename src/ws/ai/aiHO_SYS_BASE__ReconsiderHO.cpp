#include "../../headers/ws/ai/aiHO_SYS_BASE.h"
#include "../../headers/ws/ai/aiCON_CB_globals.h" // aiPLANNER *aiPlanner

// aiHO_SYS_BASE::ReconsiderHO @ 0x832A58C0
// ?ReconsiderHO@aiHO_SYS_BASE@@UAAFFF@Z
//
// Base default: ping the planner's waypoint accessor with (howpid, wpidIgnore) for its side effect
// and echo `howpid` back. The mangling takes two shorts (the decompiler widened howpid to int).
// The call is a virtual dispatch on the global planner at vtbl byte-offset 0x1C8 (IDA labels the
// slot "GetWP"); reproduced by explicit slot dispatch since aiPLANNER's vtable is a boundary.
short aiHO_SYS_BASE::ReconsiderHO(short howpid, short wpidIgnore)
{
    typedef void (*GetWPSlotFn)(aiPLANNER *, short, short);
    void **vtbl = *reinterpret_cast<void ***>(aiPlanner);
    GetWPSlotFn slot = reinterpret_cast<GetWPSlotFn>(vtbl[0x1C8 / 4]);
    slot(aiPlanner, howpid, wpidIgnore);
    return howpid;
}
