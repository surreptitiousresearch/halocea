/* system_realloc @0x83762E58 — DEVIATION: the decompiler badly mis-rendered the branch structure (it showed
 * GlobalAlloc's result being discarded, then unconditionally testing `size`). disasm_range(0x83762E58,
 * 0x83762EAC) shows three genuine tail-branches: !pointer -> return GlobalAlloc(0,size); !size ->
 * LocalFree(pointer), return 0; else -> return GlobalReAlloc(pointer,size,GMEM_MOVEABLE). */

#include <windows.h>

void * system_realloc(void *pointer, unsigned int size)
{
    if ( !pointer )
        return GlobalAlloc(0, size);

    if ( !size )
    {
        LocalFree(pointer);
        return 0;
    }

    return GlobalReAlloc(pointer, size, GMEM_MOVEABLE);
}
