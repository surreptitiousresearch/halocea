/* _byte_swap_data @0x83814150 — recursive, code-driven endian byte-swapper for tag structures. `codes` is a
 * swap-code stream: codes[0]=_begin_bs_array (never read), codes[1]=element count, codes[2..] the per-field opcodes. It walks
 * the opcodes `count` times over `data`, reversing each field in place, and reports how many bytes and how many
 * codes it consumed through the two out-params. Opcodes: -2/-4/-8 reverse a 2/4/8-byte field; a positive value
 * skips that many bytes; -101 ends one element; -100 recurses into a nested array using the same definition;
 * -102 recurses into a different byte_swap_definition (its pointer is the following code word). When `data` is
 * NULL only sizes are measured (no writes).
 *
 * DEVIATION: the DB/PDB prototype (5 args) is authoritative; Hex-Rays collapsed the two int* out-params into a
 * single __int64 and named the halves backwards. Disasm settles it: arg4 (r6) = total_size_in_bytes and is
 * written the byte offset; arg5 (r7) = total_size_in_codes and is written the code index.
 * DEVIATION: the -8 case is a plain 64-bit endian reversal — Hex-Rays rendered it as an unreadable pile of
 * __int128 OVERLAPPED register puns ("local variable allocation has failed"); reconstructed from disasm and
 * written as bswap64 (the analogous -4 pile was verified numerically to be a full bswap32).
 * DEVIATION: when codes[1] <= 0 the shipped code returns total_size_in_codes read from an uninitialized stack
 * slot (the -102 out-param scratch); reproduced faithfully. Undefined opcodes <= -8 loop without advancing —
 * a shipped assumption that only valid streams occur. */

#include <stdint.h>
#include "headers/byte_swap_definition.h"

void _byte_swap_data(byte_swap_definition *definition, void *data, int *codes,
        int *total_size_in_bytes, int *total_size_in_codes)
{
    char *bytes = (char *)data;
    int offset = 0;                 /* running byte offset into data */
    int code_index;                 /* running index into codes[] */
    int sub_diff_bytes;             /* -102 recursion out-param scratch (read uninitialized on the empty path) */

    if ( codes[1] <= 0 )
    {
        code_index = sub_diff_bytes;   /* faithful quirk: uninitialized (see header) */
        goto finish;
    }

    int element_count = codes[1];
    do
    {
        code_index = 2;                 /* skip the size/count header */
        char element_done = 0;
        do
        {
            int *code_ptr = &codes[code_index];
            int code = *code_ptr;

            if ( code > _8byte )
            {
                if ( code == _4byte )
                {
                    if ( data )
                    {
                        unsigned int dw = *(unsigned int *)&bytes[offset];
                        *(unsigned int *)&bytes[offset] =
                              ((dw & 0x000000FFu) << 24) | ((dw & 0x0000FF00u) << 8)
                            | ((dw & 0x00FF0000u) >> 8)  | ((dw & 0xFF000000u) >> 24);
                    }
                    offset += 4;
                }
                else if ( code == _2byte )
                {
                    if ( data )
                    {
                        unsigned short w = *(unsigned short *)&bytes[offset];
                        *(unsigned short *)&bytes[offset] = (unsigned short)((w << 8) | (w >> 8));
                    }
                    offset += 2;
                }
                else
                {
                    if ( code <= 0 )
                        continue;           /* no-op codes: re-read without advancing */
                    offset += code;         /* positive: skip that many bytes */
                }
                ++code_index;
                continue;
            }

            if ( code == _8byte )
            {
                if ( data )
                {
                    uint64_t qw = *(uint64_t *)&bytes[offset];
                    *(uint64_t *)&bytes[offset] =
                          ((qw & 0x00000000000000FFull) << 56) | ((qw & 0x000000000000FF00ull) << 40)
                        | ((qw & 0x0000000000FF0000ull) << 24) | ((qw & 0x00000000FF000000ull) << 8)
                        | ((qw & 0x000000FF00000000ull) >> 8)  | ((qw & 0x0000FF0000000000ull) >> 24)
                        | ((qw & 0x00FF000000000000ull) >> 40) | ((qw & 0xFF00000000000000ull) >> 56);
                }
                offset += 8;
                ++code_index;
                continue;
            }

            if ( code == _extern_bs_definition )     /* nested different-definition swap */
            {
                byte_swap_definition *sub = (byte_swap_definition *)code_ptr[1];
                char *sub_data = data ? &bytes[offset] : nullptr;
                _byte_swap_data(sub, sub_data, sub->codes, &sub_diff_bytes, nullptr);
                code_index += 2;
                offset += sub_diff_bytes;
                continue;
            }

            if ( code == _end_bs_array )     /* end of one element */
            {
                element_done = 1;
                ++code_index;
                continue;
            }

            if ( code != _begin_bs_array )     /* undefined opcode <= -8: shipped code re-reads without advancing */
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
                char *sub_data = data ? &bytes[offset] : nullptr;
                _byte_swap_data(definition, sub_data, code_ptr, &sub_bytes, &sub_codes);
                code_index += sub_codes;
                offset += sub_bytes;
            }
        }
        while ( !element_done );
        --element_count;
    }
    while ( element_count );

finish:
    if ( total_size_in_bytes )
        *total_size_in_bytes = offset;
    if ( total_size_in_codes )
        *total_size_in_codes = code_index;
}
