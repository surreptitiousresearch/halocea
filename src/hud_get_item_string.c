/* hud_get_item_string @0x83708820 — look up a HUD item message string by index from the HUD globals' item-
 * message string list, falling back to the shared default string when the list tag is unloaded or the index is
 * out of range. */

#include <stdint.h>
#include "headers/hud_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/blam_data_globals.h"

extern unsigned short *unicode_string_list_get_string(int tag_index, int16_t string_index);

const wchar_t *hud_get_item_string(int index)
{
    int tag_index = hud_globals->messaging.hud_item_messages.index;
    if (tag_index != -1)
    {
        int *string_list = TAG_GET(int, tag_index);
        if (string_list && index >= 0 && index < *string_list)
            return unicode_string_list_get_string(hud_globals->messaging.hud_item_messages.index, index);
    }
    return default_string;
}
