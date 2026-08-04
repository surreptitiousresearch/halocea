/* hs_parse_hud_message @0x83776DD8 — parse a HUD-message name. The messages live in the scenario's
 * hud_messages tag (a hud_message_text definition); the name block sits at offset 32 in that tag definition,
 * 64-byte elements at offset 0. If the tag reference is unresolved the parse fails. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/cache_file_tag_instance.h"
#include "headers/tag_block.h"

extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_hud_message(int expression_index)
{
    int tag_index = global_scenario->hud_messages.index;
    if ( tag_index == -1 )
        return 0;

    char *tag_blob = (char *)global_tag_instances[tag_index].base_address;
    /* Faithful residue: the hud_message_text tag-definition struct has no DB type, so its name tag_block
     * (at byte +32, 64-byte elements) stays a raw offset into the definition blob. */
    return hs_parse_scenario_datum(expression_index, 0, (tag_block *)&tag_blob[32], 64);
}
