/* preview_list_get_list_index_for_item @0x83780958 — list_index field (16-byte-entry offset +8) of row
 * `index` in the active preview list bank; out-of-range returns -1. */

#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"


int preview_list_get_list_index_for_item(int index)
{
    if (index >= 0 && index < preview_list_array_bank[preview_list_current_bank].count)
        return ((bank_item_s *)preview_list_array_bank[preview_list_current_bank].elements)[index].list_index;

    return -1;
}
