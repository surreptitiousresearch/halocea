#include <stdint.h>
#include "../headers/ws/ap/apMEM_BLOCK_TYPE.h"

/* ws-engine boundary (mangled: __apMemoryDebugAdd(void*,uint,char const*,uint,apMEM_BLOCK_TYPE)). */
int __apMemoryDebugAdd(void *mem, uint32_t size, const char *file, uint32_t line, apMEM_BLOCK_TYPE type);

/* Thin HCEX wrapper: tags a system-category allocation with the debug allocator. */
extern "C" void hcex_memory_debug_add(void *mem, uint32_t size, const char *file, uint32_t line)
{
    __apMemoryDebugAdd(mem, size, file, line, AP_MEM_BT_SYSTEM);
}
