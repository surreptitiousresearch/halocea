/* game_engine_playlist_clear @0x83749420 — free the active playlist: release each item's map and variant
 * name strings, free the item array, and reset the playlist to empty (no cursor). */

#include "headers/game_engine_playlist_s.h"

extern void dlFree(void *ptr);

void game_engine_playlist_clear(void)
{
    game_engine_playlist_item_s *playlist = game_engine_playlist.playlist;
    if ( playlist )
    {
        for ( unsigned int i = 0; i < game_engine_playlist.playlist_count; ++i )
        {
            game_engine_playlist_item_s *item = &playlist[i];
            dlFree(item->map_name);
            dlFree(item->variant_name);
            playlist = game_engine_playlist.playlist;
        }
        dlFree(playlist);
    }
    game_engine_playlist.playlist = 0;
    game_engine_playlist.playlist_max_count = 0;
    game_engine_playlist.playlist_count = 0;
    game_engine_playlist.now_playing = -1;
}
