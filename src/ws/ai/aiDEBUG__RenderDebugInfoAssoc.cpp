// ws-engine ai08 (a8_debug.cpp): aiDEBUG::RenderDebugInfoAssoc @0x83193E90 (static).
// Draw one line per waypoint-association record (vFrom->vTo), tinted `colorEnabled` when the
// association is visible and `colorDisabled` otherwise.
#include "../../headers/ws/ai/aiDEBUG.h"
#include "../../headers/ws/ai/aiDEBUG_boundaries.h"   // navASSOC_INFO / rendDrv
#include "../../headers/ws/rend/rendDRIVER.h"

void aiDEBUG::RenderDebugInfoAssoc(const navASSOC_INFO *assoc, int count,
                                   unsigned long colorEnabled, unsigned long colorDisabled)
{
    for (int i = 0; i < count; ++i) {
        unsigned long color = assoc[i].isVisible ? colorEnabled : colorDisabled;
        rendDrv->RenderLine(&assoc[i].vFrom, &assoc[i].vTo, color);
    }
}
