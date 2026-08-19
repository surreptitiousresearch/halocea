/* hs_enumerate_ai_names @0x8372F568 — enumerates the scenario's ai_encounters block for HaloScript. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


#include "headers/tag_block.h"
extern void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size);

void hs_enumerate_ai_names(void)
{
    if (global_scenario_index != -1)
        hs_enumerate_block_data(&global_scenario->ai_encounters, 0, 176);
}
