/* hs_enumerate_scenario_data @0x8372F360 — enumerates a tag_block embedded in the scenario tag (at
 * block_offset) for HaloScript, gated on a scenario actually being loaded. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/tag_block.h"
#include "headers/blam_data_globals.h"


extern void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size);

void hs_enumerate_scenario_data(int16_t block_offset, int16_t element_offset, int size)
{
    if (global_scenario_index != -1)
        hs_enumerate_block_data((tag_block *)((char *)global_scenario + block_offset), element_offset, size);
}
