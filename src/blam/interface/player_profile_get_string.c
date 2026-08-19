/* player_profile_get_string @0x83781678 — tail-call forwarder (`b single_preview_column_list_get_string`;
 * confirmed via disasm, not a bare blr). Same reuse as gametype_select_get_string/solo_map_get_string —
 * all three list-box "get string" callbacks share one generic implementation. Declared extern with the
 * target's own DB prototype and called through with the same arguments. */

#include <stdint.h>

extern uint8_t single_preview_column_list_get_string(uint16_t *out, int index, void *list_items);

uint8_t player_profile_get_string(uint16_t *out, int index, void *list_items)
{
    return single_preview_column_list_get_string(out, index, list_items);
}
