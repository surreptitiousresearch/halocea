/* hs_enumerate_ai_command_list_names @0x8372F598 — enumerates the scenario's ai_command_lists block for HaloScript. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"


#include "headers/tag_block.h"
extern void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size);

void hs_enumerate_ai_command_list_names(void)
{
    if (global_scenario_index != -1)
        hs_enumerate_block_data(&global_scenario->ai_command_lists, 0, 96);
}
