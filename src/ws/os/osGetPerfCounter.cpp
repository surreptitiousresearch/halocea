// FUNCTION_INDEX entry: osGetPerfCounter @0x825E5850 (?osGetPerfCounter@@YA_KXZ)
#include <stdint.h>
#include "../../headers/ws/os/os_boundary.h"

// DEVIATION: the decompiler rendered this as returning only `counter.LowPart` (32-bit), but
// the disasm is unambiguous: `ld r3, 0x60+var_10(r1)` is a 64-bit load-doubleword of the full
// LARGE_INTEGER into r3 -- Xenon's ABI returns 64-bit values whole in r3 (64-bit GPRs even in
// 32-bit PPC mode). This matches the caller osTIMER2::Snapshot, which already declares/consumes
// this as `unsigned __int64`. Reconstructed as the full QuadPart return.
uint64_t osGetPerfCounter()
{
    _LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}
