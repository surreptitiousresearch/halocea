/* single_preview_column_list_get_string @0x837812E8 — fetches the display string for row `index` of the
 * "single preview column" list UI (shared by gametype_select_get_string/player_profile_get_string/
 * solo_map_get_string via tail-call forwarding), truncated into caller's 64-wchar_t `out` buffer.
 * `list_items` is read by none of these three callers and is unused here too. */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string; /* .short 0 — empty wide string, "tag not loaded" fallback */
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);


uint8_t single_preview_column_list_get_string(uint16_t *out, int index, void *list_items)
{
    const wchar_t *text = &empty_wide_string;

    if ( index >= 0 && index < preview_list_array_bank[preview_list_current_bank].count )
        text = ((const wchar_t **)preview_list_array_bank[preview_list_current_bank].elements)[4 * index];

    ustrncpy(out, text, 0x3F);
    out[63] = 0;
    return out[0] != 0;
}
