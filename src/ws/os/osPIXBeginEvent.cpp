// FUNCTION_INDEX entry: osPIXBeginEvent @0x825E56D0 (?osPIXBeginEvent@@YAXPBD@Z)
#include "../../headers/ws/os/os_boundary.h"

// Pushes a PIX named-event scope. An empty/null name is replaced with a fallback label so PIX
// captures always show a readable marker.
void osPIXBeginEvent(const char *name)
{
    const char *eventName = name;
    const char *scan = name;
    while (*scan++)
        ;
    if (scan - name == 1) // measured length 0 -- empty string
        eventName = "No name";
    PIXBeginNamedEvent_Copy_NoVarArgs(0, eventName);
    ++osPixEventDepth;
}
