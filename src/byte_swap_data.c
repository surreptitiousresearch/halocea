/* byte_swap_data @0x838143B8 — top-level array byte-swapper (no PDB name; DB gives a 3-arg prototype).
 * NAMED byte_swap_data: signature (definition, address, count) and the byte_swap_data_explicit wrapper
 * dispatching here match Blam byte_swapping.c's public byte_swap_data entry exactly; the recursive helper
 * already holds the underscore name _byte_swap_data. Swaps `data_count` consecutive structures of type
 * `definition` in place, each `definition->size` bytes, by walking the definition's swap-code stream per
 * element. When `data` is NULL it does nothing (the recursive size-measuring path lives in the
 * _byte_swap_data helper). This is the entry point; nested array (-100) and nested-definition (-102)
 * codes recurse into _byte_swap_data (@0x83814150). Opcodes match that
 * helper: -2/-4/-8 reverse a 2/4/8-byte field, positive skips, -101 ends an element.
 *
 * DEVIATION: the DB prototype (3 args) is authoritative; Hex-Rays added a phantom __int64 `a4` — that is the
 * OVERLAPPED swap-scratch register, not an argument.
 * DEVIATION: the -8 case is a 64-bit endian reversal that Hex-Rays rendered as an __int128 register-pun pile
 * ("local variable allocation has failed"); reconstructed as bswap64 (the -4 pile is the equivalent bswap32).
 * Undefined opcodes <= -8 loop without advancing — a shipped assumption that only valid streams occur. */

#include <stdint.h>
#include "headers/byte_swap_definition.h"

extern void _byte_swap_data(byte_swap_definition *definition, void *data, int *codes,
        int *total_size_in_bytes, int *total_size_in_codes);

void byte_swap_data(byte_swap_definition *definition, void *data, int data_count)
{
    if ( !data )
        return;

    char *bytes = (char *)data;
    for ( int element = 0; element < data_count; ++element )
    {
        int *codes = definition->codes;
        int offset = 0;
        char *element_data = &bytes[definition->size * element];

        if ( codes[1] <= 0 )
            continue;

        int element_count = codes[1];
        do
        {
            int code_index = 2;
            char element_done = 0;
            do
            {
                int *code_ptr = &codes[code_index];
                int code = *code_ptr;

                if ( code > -8 )
                {
                    if ( code == -4 )
                    {
                        unsigned int dw = *(unsigned int *)&element_data[offset];
                        *(unsigned int *)&element_data[offset] =
                              ((dw & 0x000000FFu) << 24) | ((dw & 0x0000FF00u) << 8)
                            | ((dw & 0x00FF0000u) >> 8)  | ((dw & 0xFF000000u) >> 24);
                        offset += 4;
                    }
                    else if ( code == -2 )
                    {
                        unsigned short w = *(unsigned short *)&element_data[offset];
                        *(unsigned short *)&element_data[offset] = (unsigned short)((w << 8) | (w >> 8));
                        offset += 2;
                    }
                    else
                    {
                        if ( code <= 0 )
                            continue;
                        offset += code;
                    }
                    ++code_index;
                    continue;
                }

                if ( code == -8 )
                {
                    uint64_t qw = *(uint64_t *)&element_data[offset];
                    *(uint64_t *)&element_data[offset] =
                          ((qw & 0x00000000000000FFull) << 56) | ((qw & 0x000000000000FF00ull) << 40)
                        | ((qw & 0x0000000000FF0000ull) << 24) | ((qw & 0x00000000FF000000ull) << 8)
                        | ((qw & 0x000000FF00000000ull) >> 8)  | ((qw & 0x0000FF0000000000ull) >> 24)
                        | ((qw & 0x00FF000000000000ull) >> 40) | ((qw & 0xFF00000000000000ull) >> 56);
                    offset += 8;
                    ++code_index;
                    continue;
                }

                if ( code == -102 )     /* nested different-definition swap */
                {
                    byte_swap_definition *sub = (byte_swap_definition *)code_ptr[1];
                    int sub_diff_bytes;
                    _byte_swap_data(sub, &element_data[offset], sub->codes, &sub_diff_bytes, nullptr);
                    code_index += 2;
                    offset += sub_diff_bytes;
                    continue;
                }

                if ( code == -101 )     /* end of one element */
                {
                    element_done = 1;
                    ++code_index;
                    continue;
                }

                if ( code != -100 )     /* undefined opcode <= -8: re-read without advancing */
                {
                    if ( code <= 0 )
                        continue;
                    offset += code;
                    ++code_index;
                    continue;
                }

                /* code == -100: nested same-definition array */
                {
                    int sub_bytes;
                    int sub_codes;
                    _byte_swap_data(definition, &element_data[offset], code_ptr, &sub_bytes, &sub_codes);
                    code_index += sub_codes;
                    offset += sub_bytes;
                }
            }
            while ( !element_done );
            --element_count;
        }
        while ( element_count );
    }
}
