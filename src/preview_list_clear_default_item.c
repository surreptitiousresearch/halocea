/* preview_list_clear_default_item @0x83780A28 — despite the name, disasm_range(0x83780A28,0x83780A8C)
 * confirms this never writes anything: it just scans every bank's items for one whose byte+0xC flag is set
 * and returns immediately (`bnelr`) when found, otherwise falls through after exhausting all 3 banks. */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"


void preview_list_clear_default_item(void)
{
    if ( !preview_list_has_default )
        return;

    for ( int bank = 0; bank < 3; bank++ )
    {
        int count = preview_list_array_bank[bank].count;
        char *elements = (char *)preview_list_array_bank[bank].elements;

        for ( int i = 0; i < count; i++ )
        {
            if ( elements[16 * i + 12] )
                return;
        }
    }
}
