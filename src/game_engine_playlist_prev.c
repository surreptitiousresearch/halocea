/* game_engine_playlist_prev @0x8374956C — step the multiplayer playlist cursor back one entry (wrapping
 * to the end), and load the new entry's map name and game variant into the pending play stage.
 *
 * DEVIATION: the decompiler types the result as void* (the trailing memcpy return); callers ignore it. */

#include "headers/game_engine_playlist_s.h"
#include "headers/play_stage.h"
#include <string.h>

void *game_engine_playlist_prev(void)
{
    if ( game_engine_playlist.playlist_count )
    {
        int now_playing = game_engine_playlist.now_playing - 1;
        if ( now_playing < 0 )
            now_playing = game_engine_playlist.playlist_count - 1;
        game_engine_playlist.now_playing = now_playing;

        strncpy(global_stage.map_name, game_engine_playlist.playlist[now_playing].map_name, 0x3Fu);
        global_stage.map_name[63] = 0;
        return memcpy(&global_stage.game_variant,
                      &game_engine_playlist.playlist[now_playing].variant,
                      sizeof(global_stage.game_variant));
    }
    return 0;
}
