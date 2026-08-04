#include <stdint.h>
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_block.h"
#include "headers/hud_state_messages.h"
#include "headers/blam_data_globals.h"

#include "headers/tag_block.h"
extern void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size);

void hs_enumerate_hud_messages(void)
{
    int index = global_scenario->hud_messages.index;
    if ( index != -1 )
    {
        hud_state_messages *definition = TAG_GET(hud_state_messages, index);
        hs_enumerate_block_data(&definition->messages, 0, 64);
    }
}
