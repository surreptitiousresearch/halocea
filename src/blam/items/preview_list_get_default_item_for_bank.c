/* preview_list_get_default_item_for_bank @0x837809B0 — index of the first item in `bank` whose
 * is_default flag is set, or -1 if none / no default exists yet. */

#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"


int preview_list_get_default_item_for_bank(int bank)
{
    if ( preview_list_has_default )
    {
        int count = preview_list_array_bank[bank].count;
        bank_item_s *items = (bank_item_s *)preview_list_array_bank[bank].elements;

        for ( int i = 0; i < count; i++ )
        {
            if ( items[i].is_default )
                return i;
        }
    }
    return -1;
}
