#include "../../headers/ws/os/os_boundary.h"

// Pops one PIX named-event scope. Pairs with osPIXBeginEvent.
void osPIXEndEvent()
{
    --osPixEventDepth;
    PIXEndNamedEvent();
}
