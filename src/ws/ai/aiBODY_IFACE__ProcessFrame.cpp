#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"
#include "../../headers/ws/ai/aiHO_SYS_BASE.h"

// aiBODY_IFACE::ProcessFrame(float) @ 0x832AB898
// ?ProcessFrame@aiBODY_IFACE@@UAAXM@Z
//
// Per-frame body update: tick the weapon module, then (only while the brain is still alive) tick
// the hideout system.
void aiBODY_IFACE::ProcessFrame(float dt)
{
    wpn.pointee->ProcessFrame(dt);
    if (!pBrain->IsDead())
        hoSys.pointee->ProcessFrame(dt);
}
