/* hs_compile_add_source @0x83775D0C — appends a chunk of HaloScript source text to the compiler's growing
 * source buffer (reallocated to fit + a null terminator). Returns a pointer to where the new chunk begins
 * within the buffer, or null on allocation failure. */

#include <string.h>
#include "headers/hs_compile_globals.h"

extern void *dlRealloc(void *ptr, unsigned int size, const char *file, unsigned int line);

char * hs_compile_add_source(int source_size, const char *source_data)
{
    char *appended_at = nullptr;

    char *new_buffer = dlRealloc(hs_compile_globals.compiled_source,
        hs_compile_globals.compiled_source_size + source_size + 1,
        "D:\\Projects\\code\\HCEX\\sources\\hs\\hs_compile.c", 0xFEu);

    if ( new_buffer )
    {
        hs_compile_globals.compiled_source = new_buffer;
        appended_at = &new_buffer[hs_compile_globals.compiled_source_size];
        memcpy(&new_buffer[hs_compile_globals.compiled_source_size], source_data, source_size);
        hs_compile_globals.compiled_source_size += source_size;
        hs_compile_globals.compiled_source[hs_compile_globals.compiled_source_size] = 0;
    }

    return appended_at;
}
