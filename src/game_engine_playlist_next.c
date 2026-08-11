/* game_engine_playlist_next @0x837494C8 — advances the multiplayer playlist cursor (wrapping at the
 * end) and copies the now-playing entry's map name and variant into the launch stage. */

#include <string.h>
#include "headers/game_engine_playlist_s.h"
#include "headers/play_stage.h"

/* memcpy declared by <string.h> */

void game_engine_playlist_next(void)
{
    if (!game_engine_playlist.playlist_count)
        return;

    unsigned int now_playing = game_engine_playlist.now_playing + 1;
    game_engine_playlist.now_playing = now_playing;
    if (now_playing >= game_engine_playlist.playlist_count)
    {
        now_playing = 0;
        game_engine_playlist.now_playing = 0;
    }

    strncpy(global_stage.map_name, game_engine_playlist.playlist[now_playing].map_name, 0x3F);
    global_stage.map_name[63] = 0;
    memcpy(&global_stage.game_variant, &game_engine_playlist.playlist[now_playing].variant,
           sizeof(global_stage.game_variant));
}
