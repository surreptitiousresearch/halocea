/* sv_unban @0x83766F30 — remove the ban-list entry at the given index and rewrite the ban-list file.
 * Each entry is 46 bytes: a 12-wide right-justified name field at +0 and a 32-wide left-justified hash
 * field at +13, written back as "name,hash" lines. */

#include "headers/dynamic_array.h"
#include "headers/blam_data_globals.h"

struct _iobuf;

extern void dynamic_array_delete_element(dynamic_array *array, int index);
extern struct _iobuf * banlist_open_file(const char *how);
/* fprintf provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
/* fclose provided by CRT via <stdio.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */

void sv_unban(int index)
{
    char removed = 0;
    if ( index >= 0 && index < banned_players.count )
    {
        dynamic_array_delete_element(&banned_players, index);
        removed = 1;
    }
    if ( removed )
    {
        struct _iobuf *file = banlist_open_file("wt");
        if ( file )
        {
            int written = 0;
            if ( banned_players.count > 0 )
            {
                int offset = 0;
                do
                {
                    fprintf(
                        file,
                        "%*s,%-*s\n",
                        12,
                        (const char *)banned_players.elements + offset,
                        32,
                        (const char *)banned_players.elements + offset + 13);
                    ++written;
                    offset += 46;
                }
                while ( written < banned_players.count );
            }
            fclose(file);
        }
    }
}
