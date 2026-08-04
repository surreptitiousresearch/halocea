/* _byte_swap_memory @0x83814040 — reverse the endianness of `count` fixed-width words in place. The `code`
 * selects the word size and is one of the byte_swap_code opcodes (_8byte / _4byte / _2byte, values -8/-4/-2);
 * any other code is a no-op.
 *
 * DEVIATION: an earlier reconstruction modelled `code` as a uint64 whose high dword held the size, testing
 * `code >> 32`. Disasm (0x83814040: cmpwi r5,-8 / -4 / -2) shows `code` is a plain 32-bit int compared
 * directly, so it is restored as `int code` bound to the byte_swap_code enum. */

#include <stdint.h>
#include "headers/byte_swap_definition.h"

/* attest: `memory` is an untyped in-place buffer (r3 only ever a load/store base for
 * lhz/sthu, lwz/stwu, ld/stdu); spelled void* per DB prototype so struct-pointer
 * callers (data_file_*, cache_hardware_format_character) pass without casts. */
void _byte_swap_memory(void *memory, int count, int code)
{
    if (code == _8byte)
    {
        /* swap 8-byte (64-bit) words */
        uint64_t *p = (uint64_t *)memory;
        while (count-- > 0)
        {
            uint64_t v = *p;
            *p = ((v & 0x00000000000000FFull) << 56)
               | ((v & 0x000000000000FF00ull) << 40)
               | ((v & 0x0000000000FF0000ull) << 24)
               | ((v & 0x00000000FF000000ull) <<  8)
               | ((v & 0x000000FF00000000ull) >>  8)
               | ((v & 0x0000FF0000000000ull) >> 24)
               | ((v & 0x00FF000000000000ull) >> 40)
               | ((v & 0xFF00000000000000ull) >> 56);
            ++p;
        }
    }
    else if (code == _4byte)
    {
        /* swap 4-byte (32-bit) words */
        uint32_t *p = (uint32_t *)memory;
        while (count-- > 0)
        {
            uint32_t v = *p;
            *p = ((v & 0x000000FF) << 24)
               | ((v & 0x0000FF00) <<  8)
               | ((v & 0x00FF0000) >>  8)
               | ((v & 0xFF000000) >> 24);
            ++p;
        }
    }
    else if (code == _2byte)
    {
        /* swap 2-byte (16-bit) words */
        uint16_t *p = (uint16_t *)memory;
        while (count-- > 0)
        {
            uint16_t v = *p;
            *p = (uint16_t)((v << 8) | (v >> 8));
            ++p;
        }
    }
}
