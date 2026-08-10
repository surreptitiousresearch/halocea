/* hs_parse_hud_message @0x83776DD8 — parse a HUD-message name. The messages live in the scenario's
 * hud_messages tag (a hud_state_messages definition); the name block is that definition's `messages`
 * tag_block (byte 0x20), 64-byte elements whose name starts at offset 0. If the tag reference is
 * unresolved the parse fails. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/tag_block.h"
#include "headers/hud_state_messages.h"

extern int hs_parse_scenario_datum(int expression_index, int16_t offset, tag_block *block, int element_size);

int hs_parse_hud_message(int expression_index)
{
    int tag_index = global_scenario->hud_messages.index;
    if ( tag_index == -1 )
        return 0;

    /* DEVIATION: tag_reference.index holds a full tag datum handle (salt in the high word); only its
     * low word is the absolute table index. @0x83776DF4 `clrlslwi r10, r11, 16,5` clears the high 16
     * bits before applying the <<5 (32-byte cache_file_tag_instance) stride, so the subscript is
     * (uint16_t)tag_index — i.e. TAG_GET. The recovered source indexed with the whole 32-bit handle.
     * DEVIATION: byte +0x20 is hud_state_messages.messages (DB-verified layout), the same member
     * hs_enumerate_hud_messages resolves off this same tag reference — not an untyped blob offset. */
    hud_state_messages *definition = TAG_GET(hud_state_messages, tag_index);
    return hs_parse_scenario_datum(expression_index, 0, &definition->messages, 64);
}
