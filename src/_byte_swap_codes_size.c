/* _byte_swap_codes_size @0x83814360 — measure the in-memory byte size of one structure described
 * by a swap-code stream. Builds a transient byte_swap_definition around `codes` (size unknown, so
 * 0) and runs the recursive measuring pass of _byte_swap_data with data == NULL, which walks the
 * codes without touching memory and reports the total field byte count. */

#include "headers/byte_swap_definition.h"

extern void _byte_swap_data(byte_swap_definition *definition, void *data, int *codes,
        int *total_size_in_bytes, int *total_size_in_codes);

int _byte_swap_codes_size(char *name, int *codes)
{
    int total_size_in_bytes;
    int total_size_in_codes[3];   /* out-param scratch (only [0] is written) */
    byte_swap_definition definition;

    definition.codes = codes;
    definition.name = name;
    definition.size = 0;
    definition.signature = BYTE_SWAP_DEFINITION_SIGNATURE;

    _byte_swap_data(&definition, nullptr, codes, &total_size_in_bytes, total_size_in_codes);

    return total_size_in_bytes;
}
