/* ?Lock@osLOCK@@QAAXPBDH@Z @0x825E6978 */
#include "osLOCK.h"
#include "os_boundary.h"

// 0x825E6978
// CAVEAT: this function only READS threadNmb, it never WRITES it (confirmed against disasm --
// no store to offset 0x21 anywhere in the body). The `threadNmb == 0xFF` check therefore reads
// as "no recursive owner claimed yet"; when it is anything else, the function calls
// GetCurrentProcessorNumber() and returns WITHOUT entering the critical section or advancing
// lockDepth. Reproduced verbatim from disasm -- this looks like an incomplete/stubbed
// recursive-lock path in the shipped binary rather than a decompiler misread.
void osLOCK::Lock(const char *file, int line)
{
    if (!isInited) {
        RtlInitializeCriticalSection(data);
        isInited  = true;
        lockDepth = 0;
    }

    if ((unsigned char)threadNmb == 0xFF) {
        RtlEnterCriticalSection(data);
        int enteredLockDepth = lockDepth;
        locker_file = file;
        locker_line = line;
        lockDepth   = enteredLockDepth + 1;
    } else {
        GetCurrentProcessorNumber();
    }
}
