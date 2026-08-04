#include "../../headers/ws/ai/aiBODY_IFACE.h"
#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiWEAPON_BASE.h"

// 0x832ABF20 ?FillCache@aiBODY_IFACE@@UAAXXZ
// Base per-frame cache refresh: when the brain is alive and a weapon module is present, refresh the
// weapon module's cached vars. (The binary also lazily constructs a debug "tester" static local on
// first call — compiler static-init scaffolding with no observable effect — omitted here.)
void aiBODY_IFACE::FillCache()
{
    if (!this->pBrain->IsDead() && this->wpn.pointee)
        this->wpn.pointee->UpdateVarsCache();
}
