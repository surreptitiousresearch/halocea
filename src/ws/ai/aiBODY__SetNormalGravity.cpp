#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ap/apCOUNTER_TIME.h"
#include "../../headers/ws/m3d/m3dV.h"

// aiBODY::SetNormalGravity(const m3dV*) @ 0x832AC9F0
// ?SetNormalGravity@aiBODY@@UAAXPBUm3dV@@@Z
//
// Restore normal gravity on the body. In this build the routine reduces to a profiled empty scope
// (the gravity write is compiled out); only the per-call time counter is sampled.
void aiBODY::SetNormalGravity(const m3dV *grav)
{
    static apCOUNTER_TIME localTimeCounter("process_frame/ai/brain/gs/init/1G_fsm_event", nullptr);
    apCOUNTER_TIME_CALL_WRP<apCOUNTER_TIME> timeScope(&localTimeCounter);
    (void)grav;
}
