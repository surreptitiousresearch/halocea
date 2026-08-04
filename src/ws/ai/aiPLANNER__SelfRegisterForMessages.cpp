// aiPLANNER::SelfRegisterForMessages @ 0x832460A8
// ?SelfRegisterForMessages@aiPLANNER@@QAAXXZ
#include "../../headers/ws/ai/aiPLANNER.h"
#include "../../headers/ws/ai/aiEXIFACE.h"     // aiPlanner (global aiPLANNER*)
#include "../../headers/ws/msg/msgSUBSCRIBER.h"

// ?physEventHandlerBase@@3VmsgSUBSCRIBER@@A — the physics/event message registry the planner
// subscribes to. Its subscriber address is the global planner, whose iface embeds a msgADDR at
// offset 0 (hence the reinterpret cast to msgADDR*, matching the decompile).
extern msgSUBSCRIBER physEventHandlerBase;

// Subscribe the planner to the four physics-event message ids it services (1023..1026).
void aiPLANNER::SelfRegisterForMessages()
{
    physEventHandlerBase.RegisterPair((msgADDR *)aiPlanner, 1024);
    physEventHandlerBase.RegisterPair((msgADDR *)aiPlanner, 1023);
    physEventHandlerBase.RegisterPair((msgADDR *)aiPlanner, 1025);
    physEventHandlerBase.RegisterPair((msgADDR *)aiPlanner, 1026);
}
