/* hs_enumerate_block_data @0x8372F2DC — for each element of a tag_block, treat the string at element+element_offset
 * as a HaloScript global name and register it with the tokenizer's enumeration, unless it names an external
 * global that is NOT currently registered in hs_external_globals.
 *
 * Surprising-but-verified quirk: when hs_find_global_by_name finds an external global it returns (i | 0x8000)
 * sign-extended to a NEGATIVE __int16 (see hs_find_global_by_name.c). This function indexes
 * hs_external_globals[] with that raw negative value directly (confirmed via disasm — no `& 0x7FFF` mask is
 * present), rather than recovering the plain index `i` first. Reproduced literally rather than "corrected",
 * since the disasm shows this is the compiled behavior, not a decompiler translation artifact. */

#include <stdint.h>
#include "headers/tag_block.h"
#include "headers/hs_global_definition.h"
#include "headers/blam_data_globals.h"

extern char *tag_block_get_element_with_size(const tag_block *block, int index, int element_size);
extern int16_t hs_find_global_by_name(const char *name);
extern void hs_tokens_enumerate_add_string(const char *string);

void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size)
{
    if ( block->count <= 0 )
        return;

    for ( int16_t i = 0; i < block->count; i++ )
    {
        const char *name = tag_block_get_element_with_size(block, i, element_size) + element_offset;
        int global_by_name = hs_find_global_by_name(name);

        if ( global_by_name == -1 || (global_by_name < 0 && hs_external_globals[global_by_name]) )
            hs_tokens_enumerate_add_string(name);
    }
}
