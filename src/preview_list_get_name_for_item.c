/* preview_list_get_name_for_item @0x837808A8 — name field (16-byte-entry offset 0) of row `index` in the
 * active preview list bank; out-of-range returns the shared empty-string fallback. */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string; /* .short 0 — empty wide string fallback */

const wchar_t * preview_list_get_name_for_item(int index)
{
    if (index >= 0 && index < preview_list_array_bank[preview_list_current_bank].count)
        return ((const wchar_t **)preview_list_array_bank[preview_list_current_bank].elements)[4 * index];

    return &empty_wide_string;
}
