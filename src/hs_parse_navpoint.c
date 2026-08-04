/* hs_parse_navpoint @0x83776D58 — parse a navpoint name. Navpoints live in the HUD globals tag (interface
 * group index 6 via interface_get_tag_index); the name block is hud_globals_definition.waypoint.arrows
 * (offset 352 in the tag definition), 104-byte hud_waypoint_arrow elements whose name string starts at
 * offset 0. If the interface tag is absent the parse fails. */

#include <stdint.h>
#include "headers/cache_file_tag_instance.h"
#include "headers/tag_block.h"
#include "headers/hud_globals_definition.h"
#include "headers/interface_tag_index.h"

#include "headers/tag_block.h"
extern int interface_get_tag_index(int16_t interface_tag_index);
extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_navpoint(int expression_index)
{
    if ( interface_get_tag_index(_interface_hud_globals) == -1 )
        return 0;

    int tag_index = interface_get_tag_index(_interface_hud_globals);
    hud_globals_definition *hud_globals = (hud_globals_definition *)global_tag_instances[tag_index].base_address;
    return hs_parse_scenario_datum(expression_index, 0, &hud_globals->waypoint.arrows, 104);
}
