#include "../../headers/ws/os/os_boundary.h"

// PowerPC lightweight-sync memory barrier (Xenon intrinsic __lwsync -> `lwsync` instruction).
void osMemoryBarrier()
{
    __lwsync();
}
