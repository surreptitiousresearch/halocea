/* preview_list_clear_default_item @0x83780A28 — despite the name, disasm_range(0x83780A28,0x83780A8C)
 * confirms this never writes anything (re-verified 2026-08-05: not one store instruction in the range):
 * it just scans every bank's items for one whose is_default flag is set and returns immediately (`bnelr`)
 * when found, otherwise falls through after exhausting all 3 banks. */

#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"


void preview_list_clear_default_item(void)
{
    if ( !preview_list_has_default )
        return;

    for ( int bank = 0; bank < 3; bank++ )
    {
        int count = preview_list_array_bank[bank].count;
        bank_item_s *items = (bank_item_s *)preview_list_array_bank[bank].elements;

        for ( int i = 0; i < count; i++ )
        {
            if ( items[i].is_default )
                return;
        }
    }
}
