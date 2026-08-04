#include "../../headers/ws/ai/aiBRAIN.h"
#include "../../headers/ws/ai/aiBODY_IFACE.h" // body.pointee->PreProcessInit()

// aiBRAIN::PreProcessINIT() @ 0x832AD810
// ?PreProcessINIT@aiBRAIN@@UAAXXZ
//
// Virtual per-spawn pre-process init: construct the brain's body/mind subsystems, then run the
// body interface's own pre-process init.
void aiBRAIN::PreProcessINIT()
{
    SetupSubSystems();
    body.pointee->PreProcessInit();
}
