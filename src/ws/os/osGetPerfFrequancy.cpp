#include <stdint.h>
#include "../../headers/ws/os/os_boundary.h"

// DEVIATION: same LowPart-only decompiler artifact as osGetPerfCounter -- the disasm's
// `ld r3, 0x60+var_10(r1)` loads the full 64-bit LARGE_INTEGER. Reconstructed as the full
// QuadPart return, matching the caller osTIMER2::Snapshot's `unsigned __int64` usage.
// NOTE: "Frequancy" is the shipped misspelling of the real symbol name -- kept verbatim.
uint64_t osGetPerfFrequancy()
{
    _LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}
