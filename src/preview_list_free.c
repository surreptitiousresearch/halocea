/* preview_list_free @0x83780738 — free the two dlMalloc'd pointers (name/data, offsets +0/+4) of every row
 * in each of the 3 preview list banks (16-byte elements, see preview_list_create.c/
 * preview_list_get_data_for_item.c), then dispose the banks themselves. */

#include "headers/dynamic_array.h"
#include "headers/bank_item_s.h"
#include "headers/blam_data_globals.h"

extern void dlFree(void *ptr);

void preview_list_free(void)
{
    for ( int bank = 0; bank < 3; ++bank )
    {
        for ( int i = 0; i < preview_list_array_bank[bank].count; ++i )
        {
            bank_item_s *entry = &((bank_item_s *)preview_list_array_bank[bank].elements)[i];
            if ( entry->name )
                dlFree(entry->name);
            if ( entry->ptr )
                dlFree(entry->ptr);
        }

        dynamic_array_delete(&preview_list_array_bank[bank]);
    }
}
