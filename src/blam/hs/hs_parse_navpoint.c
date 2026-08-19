/* hs_parse_navpoint @0x83776D58 — parse a navpoint name. Navpoints live in the HUD globals tag (interface
 * group index 6 via interface_get_tag_index); the name block is hud_globals_definition.waypoint.arrows
 * (offset 352 in the tag definition), 104-byte hud_waypoint_arrow elements whose name string starts at
 * offset 0. If the interface tag is absent the parse fails. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
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
    /* DEVIATION: interface_get_tag_index returns tag_reference.index verbatim (@0x836A64C4
     * `lwz r3, 0xC(r9)`), i.e. a full tag datum handle with salt in the high word; only its low word
     * is the absolute table index. @0x83776D90 `clrlslwi r10, r3, 16,5` clears the high 16 bits
     * before applying the <<5 (32-byte cache_file_tag_instance) stride, so the subscript is
     * (uint16_t)tag_index — i.e. TAG_GET. The recovered source indexed with the whole 32-bit handle.
     * The `== -1` test above is on the unmasked handle, matching `cmpwi cr6, r3, -1` @0x83776D7C. */
    hud_globals_definition *hud_globals = TAG_GET(hud_globals_definition, tag_index);
    return hs_parse_scenario_datum(expression_index, 0, &hud_globals->waypoint.arrows, 104);
}
