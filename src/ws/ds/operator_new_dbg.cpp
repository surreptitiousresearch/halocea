// operator new(unsigned int, const char *, unsigned int) @ 0x82506968
// Debug-tracked global placement operator new: allocate `memSize` bytes (padded by a 6-byte guard,
// clamped to at least 1 on the wrap-around case), abort on OOM, then register the block with the
// memory debugger tagged {fileName, fileLine} as an allocator allocation.
#include "../../headers/ws/ap/apDLALLOC_IFACE.h"
#include "../../headers/ws/ap/apMEM_BLOCK_TYPE.h"

extern "C" void dlDumpNoMemLeft(void); // boundary — OOM diagnostic + abort
// boundary — mangled __apMemoryDebugAdd(void*, uint, char const*, uint, apMEM_BLOCK_TYPE)
int __apMemoryDebugAdd(void *mem, unsigned int size, const char *file, unsigned int line,
                       apMEM_BLOCK_TYPE type);

// first param must be size_t per C++ (gate is x64); original is 32-bit unsigned int
void *operator new(size_t memSize, const char *fileName, unsigned int fileLine)
{
    unsigned int allocSize = (unsigned int)memSize + 6;
    if (memSize == (unsigned int)-6) // padding overflowed to 0 — clamp to a 1-byte allocation
        allocSize = 1;

    void *mem = apDLALLOC_IFACE::Malloc(allocSize);
    if (!mem)
        dlDumpNoMemLeft();

    __apMemoryDebugAdd(mem, allocSize, fileName, fileLine, AP_MEM_BT_ALLOCATOR);
    return mem;
}
