/* hs_enumerate_variable_names @0x8372F4F4 — feeds every engine-external HaloScript global's name, and (if a
 * scenario is loaded) every scenario-local script global's name, into the token enumerator. */

#include <stdint.h>
#include "headers/hs_global_external.h"
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
extern void hs_tokens_enumerate_add_string(const char *string);
extern void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size);

void hs_enumerate_variable_names(void)
{
    for ( __int16 i = 0; i < hs_external_global_count; ++i )
        hs_tokens_enumerate_add_string(hs_external_globals[i]->name);

    if ( global_scenario_index != -1 )
        hs_enumerate_block_data(&global_scenario->hs_globals, 0, 92);
}
