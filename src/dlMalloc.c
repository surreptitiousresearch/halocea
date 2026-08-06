/* dlMalloc @0x825065C8
 *
 * DEVIATION: this TU is C++, not C. `dlMalloc` is FLAT in the binary, yet the function calls three
 * genuinely mangled symbols (?Malloc@apDLALLOC_IFACE@@SAPAXI@Z, ?dlDumpNoMemLeft@@YAXXZ,
 * ?__apMemoryDebugAdd@@YAHPAXIPBDIW4apMEM_BLOCK_TYPE@@@Z) which no C compiler can emit. The original
 * TU is D:\Projects\code\common\src.sys\Ap\ap_dlalloc.cpp (named by dlDumpNoMemLeft's assert string);
 * dlDumpNoMemLeft @0x82506568 .. dlRealloc @0x82506B68 are contiguous. The dl* API entry points are
 * the extern "C" exports of that C++ TU.
 */

#include "headers/ws/ap/apDLALLOC_IFACE.h"
#include "headers/ws/ap/apMEM_BLOCK_TYPE.h"

void dlDumpNoMemLeft(void); /* ?dlDumpNoMemLeft@@YAXXZ */
/* ap memory-debug accounting (?__apMemoryDebugAdd@@YAHPAXIPBDIW4apMEM_BLOCK_TYPE@@@Z). boundary. */
int  __apMemoryDebugAdd(void *ptr, unsigned int size, const char *file, unsigned int line,
                        apMEM_BLOCK_TYPE type);

extern "C" void *dlMalloc(unsigned int size, const char *file, unsigned int line)
{
    unsigned int padded_size;
    void        *ptr;

    padded_size = size + 6;
    if (size == (unsigned int)-6)
        padded_size = 1;
    ptr = apDLALLOC_IFACE::Malloc(padded_size);
    if (!ptr)
        dlDumpNoMemLeft();
    __apMemoryDebugAdd(ptr, padded_size, file, line, AP_MEM_BT_ALLOCATOR);
    return ptr;
}
