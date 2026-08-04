/* hs_enumerate_cutscene_recording_names @0x8372F718 — enumerates the scenario's recorded_animations block for HaloScript. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


#include "headers/tag_block.h"
extern void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size);

void hs_enumerate_cutscene_recording_names(void)
{
    if (global_scenario_index != -1)
        hs_enumerate_block_data(&global_scenario->recorded_animations, 0, 64);
}
