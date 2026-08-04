/* debug_keys_initialize @ 0x836EABB0 — clear all bound debug-key variables and
 * allocate a bitset of "is-down" flags, one bit per registered debug key. */

#include "headers/bit_vector.h"
#include "headers/debug_key.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);
extern void *memset(void *, int, unsigned int);

void debug_keys_initialize(void) /* attested void: memset r3-thread, 0/1 callers consume */
{
    int count;
    int bitset_bytes;

    count = 0;
    if ( global_debug_key_list[0].name )
    {
        do
        {
            if ( global_debug_key_list[count].variable )
                *global_debug_key_list[count].variable = 0;
            ++count;
        }
        while ( global_debug_key_list[count].name );
    }

    bitset_bytes = 4 * BIT_VECTOR_SIZE_IN_LONGS(count);
    global_debug_key_down = dlMalloc(
        bitset_bytes, "D:\\Projects\\code\\HCEX\\sources\\main\\debug_keys.c", 0x64);
    memset(global_debug_key_down, 0, bitset_bytes);
}
