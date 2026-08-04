#include "../../headers/ws/ai/aiBODY.h"
#include "../../headers/ws/ds/dsSTRID.h"
#include "../../headers/ws/m3d/m3dV.h"

extern dsSTRID eventAlert; // ?eventAlert — interned "alert" FSM event id @ 0x843271D0

// aiBODY::SendAlertEvents(m3dV) @ 0x832AB6A0
// ?SendAlertEvents@aiBODY@@UAAXUm3dV@@@Z
//
// Notify the body FSM of an alert at world position `posAlert`: first the positional-aim event, then
// the parameterless named "alert" event.
void aiBODY::SendAlertEvents(m3dV posAlert)
{
    SendFsmEventPosAim(posAlert);
    SendFsmEventByName(eventAlert);
}
