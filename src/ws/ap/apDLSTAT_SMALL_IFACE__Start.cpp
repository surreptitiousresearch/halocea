#include "apDLSTAT_SMALL_IFACE.h"
#include "apMEM_BLOCK_TYPE.h"

// Debug operator new (Saber): (size, file, line). boundary.
extern void *operator new(size_t size, const char *file, unsigned int line);
// In-place construct the memstat hash. boundary — ap subsystem.
extern void apMEM_STAT_HASH_construct(apMEM_STAT_HASH *self); // ?apMEM_STAT_HASH::apMEM_STAT_HASH
// Register a raw allocation with the memory debug tracker. boundary.
// DEVIATION: was `void ... (…, const char *tag, int owner, …)`. The binary symbol is
// ?__apMemoryDebugAdd@@YAHPAXIPBDIW4apMEM_BLOCK_TYPE@@@Z — YAH is an int return and the 4th
// parameter is `I` (unsigned int, the source line). The old spelling mangled to
// ?__apMemoryDebugAdd@@YAXPAXIPBDHW4apMEM_BLOCK_TYPE@@@Z, i.e. a C++ overload nothing defines.
extern int __apMemoryDebugAdd(void *ptr, unsigned int size, const char *tag, unsigned int fileLine,
                              apMEM_BLOCK_TYPE type);

// File-scope singleton buffer published by Start. boundary global.
extern apMEM_STAT_HASH *_apMemStat;

// apDLSTAT_SMALL_IFACE::Start @ 0x82C4C878
void apDLSTAT_SMALL_IFACE::Start()
{
    // Deviation: the decompiler leaves the non-null branch's pointer (v1) unassigned — an artifact
    // of the split new-expression. The real code publishes the freshly constructed buffer pointer.
    apMEM_STAT_HASH *memStat = (apMEM_STAT_HASH *)operator new(
        0x200200u, "D:\\Projects\\code\\common\\src.sys\\ap\\ap_dlstat_small.cpp", 0x50u);
    if (memStat)
        apMEM_STAT_HASH_construct(memStat);

    _apMemStat = memStat;
    __apMemoryDebugAdd(memStat, 0x200200u, "memstat bufs", 0, AP_MEM_BT_ALLOCATOR);
}
