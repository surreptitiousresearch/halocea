// FUNCTION_INDEX entry: osMemoryBarrier @0x825E54D0 (?osMemoryBarrier@@YAXXZ)
#include "../../headers/ws/os/os_boundary.h"

// PowerPC lightweight-sync memory barrier (Xenon intrinsic __lwsync -> `lwsync` instruction).
void osMemoryBarrier()
{
    __lwsync();
}
