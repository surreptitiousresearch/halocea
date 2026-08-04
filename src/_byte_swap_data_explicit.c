/* _byte_swap_data_explicit @0x838145D0 — byte-swap `data_count` structures of an explicitly
 * described type in place. Builds a byte_swap_definition from the caller-supplied name/size/codes
 * (marking it verified when `data` is non-NULL) and dispatches to the top-level array swapper
 * (byte_swap_data @0x838143B8). */

#include "headers/byte_swap_definition.h"

extern void byte_swap_data(byte_swap_definition *definition, void *data, int data_count);

void _byte_swap_data_explicit(char *name, int size, int *codes, int data_count, void *data)
{
    byte_swap_definition definition;

    definition.name = name;
    definition.size = size;
    definition.codes = codes;
    definition.verified = (data != nullptr);
    definition.signature = BYTE_SWAP_DEFINITION_SIGNATURE;

    byte_swap_data(&definition, data, data_count);
}
