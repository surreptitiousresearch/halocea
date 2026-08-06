/* vidAllocPhysMem @0x827CDAD0 -- ?vidAllocPhysMem@@YAPAXKKKPAHK@Z
 *
 * Allocate a block of physical (write-combined / GPU-visible) memory for the video subsystem,
 * charged against the video memory quota. A `size` with its top bit set means "grab whatever is
 * left": it is clamped to the remaining quota and further capped at 2/3 of currently-available
 * physical RAM, after which the one-shot notEnoughMem latch is set so no further physical
 * allocations succeed. Returns NULL when out of memory (quota exceeded, latch set, or
 * XPhysicalAlloc failed).
 *
 * DEVIATION: this TU is C++, not C. The binary symbol is mangled (?vidAllocPhysMem@@YAPAXKKKPAHK@Z)
 * and the function calls two more mangled symbols (?osLockedSet@@YAHPAHH@Z,
 * ?__apMemoryDebugAdd@@YAHPAXIPBDIW4apMEM_BLOCK_TYPE@@@Z), none of which a C compiler can emit.
 * DEVIATION: the signature was `(int, unsigned int, unsigned int, unsigned int *, unsigned int)`,
 * citing ?vidAllocPhysMem@@YAPAXHIIPAII@Z -- a mangling that does not exist in the binary and that
 * was derived from the C declaration rather than read from it. The real name says K K K PAH K:
 * unsigned long x3, int *, unsigned long.
 * DEVIATION: the "grab whatever is left" test is a top-bit mask, not a signed compare -- disasm
 * 0x827CDB10 `clrrwi r11, r31, 31` isolates bit 0x80000000 and branches on it.
 * DEVIATION: the decompiler models the on-stack MEMORYSTATUS as `_MEMORYSTATUS v16[3]` with a
 * manual field-by-field zero loop; that is a single MEMORYSTATUS whose dwLength is zeroed before
 * GlobalMemoryStatus fills it (GlobalMemoryStatus ignores dwLength, unlike GlobalMemoryStatusEx).
 * XPhysicalAlloc arg order confirmed by disasm 0x827CDBA4: (dwSize, ulPhysicalAddress=-1,
 * ulAlignment, flProtect).
 */

#include <windows.h>
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

/* vid.cpp file-local physical-memory accounting globals (boundary). notEnoughMem is FLAT in the
 * binary; memQuotaMb and vidMemAllocated are ?memQuotaMb@@3HA / ?vidMemAllocated@@3HA. */
extern "C" int      notEnoughMem;    /* one-shot latch: physical pool exhausted */
extern int          memQuotaMb;      /* video memory quota, in megabytes */
extern int          vidMemAllocated; /* running total of bytes handed out; `H` in the mangling */

/* Xbox 360 physical allocator (xboxkrnl export, FLAT). */
extern "C" void *XPhysicalAlloc(unsigned int dwSize, unsigned int ulPhysicalAddress,
                                unsigned int ulAlignment, unsigned int flProtect);

/* ap memory-debug accounting (?__apMemoryDebugAdd@@YAHPAXIPBDIW4apMEM_BLOCK_TYPE@@@Z). boundary. */
extern int __apMemoryDebugAdd(void *ptr, unsigned int size, const char *tag,
                              unsigned int fileLine, apMEM_BLOCK_TYPE type);

/* os atomic exchange -- returns the PREVIOUS value. boundary.
 * DEVIATION: was declared `volatile int *`, which mangles to PCH; the binary says PAH and
 * src/ws/os/osLockedSet.cpp defines it as `int *`.
 * DEVIATION: was `extern "C"`. ?osLockedSet@@YAHPAHH@Z is MANGLED, so C linkage would name a flat
 * symbol the image does not contain. */
int osLockedSet(int *pValue, int newValue);

void *vidAllocPhysMem(unsigned long size, unsigned long align, unsigned long flags,
                      int *retSize, unsigned long dbgFlags)
{
    unsigned long allocSize = size;

    if (notEnoughMem)
        return NULL;

    if (size & 0x80000000u)
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
        *retSize = (int)allocSize;

    return ptr;
}
