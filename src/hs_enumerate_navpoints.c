/* hs_enumerate_navpoints @0x8372F748 — enumerates the navpoint name block
 * (hud_globals_definition.waypoint.arrows, offset 352 in the HUD globals tag, interface group index 6 via
 * interface_get_tag_index(_interface_hud_globals)) for HaloScript. */

#include <stdint.h>
#include "headers/cache_file_tag_instance.h"
#include "headers/tag_block.h"
#include "headers/hud_globals_definition.h"
#include "headers/interface_tag_index.h"

#include "headers/tag_block.h"
extern int interface_get_tag_index(int16_t interface_tag_index);
extern void hs_enumerate_block_data(tag_block *block, int16_t element_offset, int element_size);

void hs_enumerate_navpoints(void)
{
    if (interface_get_tag_index(_interface_hud_globals) != -1)
    {
        int tag_index = interface_get_tag_index(_interface_hud_globals);
        hud_globals_definition *hud_globals = (hud_globals_definition *)global_tag_instances[tag_index].base_address;
        hs_enumerate_block_data(&hud_globals->waypoint.arrows, 0, 104);
    }
}
