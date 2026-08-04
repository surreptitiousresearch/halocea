/* color_picker_get_string @0x83781CFC — fetches the display string for row `index` of the color-picker's
 * preview list, truncated into caller's 64-wchar_t `out` buffer. Near-identical to
 * single_preview_column_list_get_string.c (same preview_list_array_bank plumbing); `list_items` is unused
 * here too. */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string;
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);


uint8_t color_picker_get_string(uint16_t *out, int index, void *list_items)
{
    const wchar_t *text = &empty_wide_string;

    if ( index >= 0 && index < preview_list_array_bank[preview_list_current_bank].count )
        text = ((const wchar_t **)preview_list_array_bank[preview_list_current_bank].elements)[4 * index];

    ustrncpy(out, text, 0x3F);
    out[63] = 0;
    return out[0] != 0;
}
