/* preview_list_get_name_for_item @0x837808A8 — name field of row `index` in the active preview list
 * bank; out-of-range returns the shared empty-string fallback.
 * DEVIATION: the decompiler's `[4 * index]` on a wchar_t ** was the 16-byte bank_item_s stride folded
 * into a pointer subscript — the disasm scales by 16 outright (0x837808EC `slwi r9, r9, 4`). */

#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"

extern const wchar_t empty_wide_string[]; /* .rdata @0x820309EC - the shared L"" literal (def: src/data/empty_wide_string.c) */

const wchar_t * preview_list_get_name_for_item(int index)
{
    if (index >= 0 && index < preview_list_array_bank[preview_list_current_bank].count)
        return ((bank_item_s *)preview_list_array_bank[preview_list_current_bank].elements)[index].name;

    return empty_wide_string;
}
