/* preview_list_get_default_item_for_bank @0x837809B0 — index of the first item in `bank` whose default
 * flag (byte+12) is set, or -1 if none / no default exists yet. Same 16-byte element stride as
 * preview_list_clear_default_item.c. */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"


int preview_list_get_default_item_for_bank(int bank)
{
    if ( preview_list_has_default )
    {
        int count = preview_list_array_bank[bank].count;
        char *elements = (char *)preview_list_array_bank[bank].elements;

        for ( int i = 0; i < count; i++ )
        {
            if ( elements[16 * i + 12] )
                return i;
        }
    }
    return -1;
}
