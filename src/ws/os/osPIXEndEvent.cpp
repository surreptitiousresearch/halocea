// FUNCTION_INDEX entry: osPIXEndEvent @0x825E5738 (?osPIXEndEvent@@YAXXZ)
#include "../../headers/ws/os/os_boundary.h"

// Pops one PIX named-event scope. Pairs with osPIXBeginEvent.
void osPIXEndEvent()
{
    --osPixEventDepth;
    PIXEndNamedEvent();
}
