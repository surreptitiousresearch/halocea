#include "osLOCK.h"
#include "os_boundary.h"

// 0x825E69F0 -- mirror of the threadNmb gate in osLOCK::Lock (see CAVEAT there): if
// threadNmb != 0xFF, this leaves the critical section untouched and just calls
// GetCurrentProcessorNumber(). Reproduced verbatim from disasm.
void osLOCK::Unlock(const char *file, int line)
{
    (void)file;
    (void)line;

    if ((unsigned char)threadNmb == 0xFF) {
        int exitedLockDepth = lockDepth - 1;
        locker_file = nullptr;
        locker_line = 0;
        lockDepth   = exitedLockDepth;
        RtlLeaveCriticalSection(data);
    } else {
        GetCurrentProcessorNumber();
    }
}
