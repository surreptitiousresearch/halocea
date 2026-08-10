/* hs_enumerate_navpoints @0x8372F748 — enumerates the navpoint name block
 * (hud_globals_definition.waypoint.arrows, offset 352 in the HUD globals tag, interface group index 6 via
 * interface_get_tag_index(_interface_hud_globals)) for HaloScript. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
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
        /* DEVIATION: interface_get_tag_index returns tag_reference.index verbatim, i.e. a full tag
         * datum handle with salt in the high word; only its low word is the absolute table index.
         * @0x8372F770 `clrlslwi r10, r3, 16,5` clears the high 16 bits before applying the <<5
         * (32-byte cache_file_tag_instance) stride, so the subscript is (uint16_t)tag_index — i.e.
         * TAG_GET. The recovered source indexed with the whole 32-bit handle. The `!= -1` test above
         * is on the unmasked handle, matching `cmpwi cr6, r3, -1` @0x8372F75C. */
        hud_globals_definition *hud_globals = TAG_GET(hud_globals_definition, tag_index);
        hs_enumerate_block_data(&hud_globals->waypoint.arrows, 0, 104);
    }
}
