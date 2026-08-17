/* ?Init@osLOCK@@AAAXXZ @0x825E5478 */
#include "osLOCK.h"
#include "os_boundary.h"

// 0x825E5478 -- lazily create the underlying platform critical section on first use.
void osLOCK::Init()
{
    if (!isInited) {
        RtlInitializeCriticalSection(data);
        isInited  = true;
        lockDepth = 0;
    }
}
