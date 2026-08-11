/* color_picker_get_string @0x83781CD8 — fetches the display string for row `index` of the color-picker's
 * preview list, truncated into caller's 64-wchar_t `out` buffer. Near-identical to
 * single_preview_column_list_get_string.c (same preview_list_array_bank plumbing); `list_items` is unused
 * here too. */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);


uint8_t color_picker_get_string(uint16_t *out, int index, void *list_items)
{
    const wchar_t *text = empty_wide_string;

    /* DEVIATION: the decompiler's `((const wchar_t **)…)[4 * index]` folded the 16-byte bank_item_s
     * stride into a 4-byte-pointee subscript; 0x83781D30 `slwi r8, r10, 4` scales by 16 outright and
     * 0x83781D38 loads offset 0, i.e. bank_item_s[index].name. */
    if ( index >= 0 && index < preview_list_array_bank[preview_list_current_bank].count )
        text = ((bank_item_s *)preview_list_array_bank[preview_list_current_bank].elements)[index].name;

    ustrncpy(out, text, 0x3F);
    out[63] = 0;
    return out[0] != 0;
}
