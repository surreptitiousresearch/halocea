/* data_file_find_item @0x83809BD0 — linear search a data file's item table for `item_name` (case-
 * insensitive), returning its index or -1 if not found or the file is empty.
 *
 * DEVIATION: the decompiler inlined data_file_get's own data_file_id-to-data_file_s* selection verbatim;
 * restored as a call to the already-recovered data_file_get.c, which is the same logic. */

#include "headers/data_file_s.h"

extern data_file_s * data_file_get(unsigned int data_file_id);
extern int stricmp(const char *a, const char *b);

int data_file_find_item(unsigned int data_file_id, const char *item_name)
{
    data_file_s *file = data_file_get(data_file_id);

    unsigned int item_count = file->header.item_count;
    if ( !item_count )
        return -1;

    for ( unsigned int i = 0; ; ++i )
    {
        if ( i < item_count )
        {
            const char *name = &file->names[file->items[i].name_offset];
            if ( name && !stricmp(name, item_name) )
                return i;
        }

        item_count = file->header.item_count;
        if ( i + 1 >= item_count )
            return -1;
    }
}
