/* banlist_unban_index @0x83766B90 — remove entry `index` from the banned-players list
 * (banned_player_s: 13-byte name + 33-byte hash, see banlist_init.c) and, if anything was removed, rewrite the banlist
 * file from the surviving entries in the same "%*s,%-*s\n" (12-wide name, 32-wide left-justified hash)
 * format banlist_save uses. Returns 1 when an entry was removed. */

#include <stdint.h>
#include "headers/dynamic_array.h"
#include "headers/banned_player_s.h"
#include "headers/blam_data_globals.h"

extern void dynamic_array_delete_element(dynamic_array *array, int index);
extern struct _iobuf * banlist_open_file(const char *how);
extern int fprintf(struct _iobuf *stream, const char *format, ...);
extern int fclose(struct _iobuf *stream);

uint8_t banlist_unban_index(int index)
{
    uint8_t removed = 0;
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
            for ( int i = 0; i < banned_players.count; i++ )
            {
                const banned_player_s *entry = &((const banned_player_s *)banned_players.elements)[i];
                fprintf(file, "%*s,%-*s\n", 12, entry->name, 32, entry->hash);
            }
            fclose(file);
        }
    }
    return removed;
}
