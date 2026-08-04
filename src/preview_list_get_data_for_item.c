/* preview_list_get_data_for_item @0x83780900 — data pointer field (16-byte-entry offset +4) of row
 * `index` in the active preview list bank; out-of-range returns null. */

#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"


void * preview_list_get_data_for_item(int index)
{
    if (index >= 0 && index < preview_list_array_bank[preview_list_current_bank].count)
        return ((bank_item_s *)preview_list_array_bank[preview_list_current_bank].elements)[index].ptr;

    return nullptr;
}
