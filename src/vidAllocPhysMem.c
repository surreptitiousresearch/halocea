/* vidAllocPhysMem @0x827CDAD0 -- ?vidAllocPhysMem@@YAPAXHIIPAII@Z
 *
 * Allocate a block of physical (write-combined / GPU-visible) memory for the video subsystem,
 * charged against the video memory quota. A negative `size` means "grab whatever is left": it is
 * clamped to the remaining quota and further capped at 2/3 of currently-available physical RAM,
 * after which the one-shot notEnoughMem latch is set so no further physical allocations succeed.
 * Returns NULL when out of memory (quota exceeded, latch set, or XPhysicalAlloc failed).
 *
 * DEVIATION: the decompiler models the on-stack MEMORYSTATUS as `_MEMORYSTATUS v16[3]` with a
 * manual field-by-field zero loop; that is a single MEMORYSTATUS whose dwLength is zeroed before
 * GlobalMemoryStatus fills it (GlobalMemoryStatus ignores dwLength, unlike GlobalMemoryStatusEx).
 * XPhysicalAlloc arg order confirmed by disasm 0x827CDBA4: (dwSize, ulPhysicalAddress=-1,
 * ulAlignment, flProtect).
 */

#include <windows.h>
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

/* vid.cpp file-local physical-memory accounting globals (boundary). */
extern int          notEnoughMem;    /* one-shot latch: physical pool exhausted */
extern int          memQuotaMb;      /* video memory quota, in megabytes */
extern unsigned int vidMemAllocated; /* running total of bytes handed out */

/* Xbox 360 physical allocator (xboxkrnl). */
extern void *XPhysicalAlloc(unsigned int dwSize, unsigned int ulPhysicalAddress,
                            unsigned int ulAlignment, unsigned int flProtect);

/* ap memory-debug accounting (?__apMemoryDebugAdd@@YAHPAXIPBDIW4apMEM_BLOCK_TYPE@@@Z). boundary. */
extern int __apMemoryDebugAdd(void *ptr, unsigned int size, const char *tag,
                              unsigned int fileLine, apMEM_BLOCK_TYPE type);

/* os atomic store (?osLockedSet@@YAXPAHH@Z). boundary. */
extern void osLockedSet(volatile int *pValue, int newValue);

void *vidAllocPhysMem(int size, unsigned int align, unsigned int flags,
                      unsigned int *retSize, unsigned int dbgFlags)
{
    unsigned int allocSize = (unsigned int)size;

    if (notEnoughMem)
        return NULL;

    if (size < 0)
    {
        MEMORYSTATUS memStatus;
        memStatus.dwLength = 0;
        allocSize = ((unsigned int)memQuotaMb << 20) - vidMemAllocated;
        GlobalMemoryStatus(&memStatus);
        if (allocSize >= 2 * (memStatus.dwAvailPhys / 3))
            allocSize = 2 * (memStatus.dwAvailPhys / 3);
        osLockedSet(&notEnoughMem, 1);
    }

    if (!allocSize)
        return NULL;

    if ((int)(((unsigned int)memQuotaMb << 20) - vidMemAllocated - allocSize) < 0)
        return NULL;

    void *ptr = XPhysicalAlloc(allocSize, 0xFFFFFFFFu, align, flags);
    if (!ptr)
        return NULL;

    const char *tag = dbgFlags ? "streaming" : "stat vid mem";
    __apMemoryDebugAdd(ptr, allocSize, tag, 0, AP_MEM_BT_SYSTEM);
    vidMemAllocated += allocSize;

    if (retSize)
        *retSize = allocSize;

    return ptr;
}
