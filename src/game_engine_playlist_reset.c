/* game_engine_playlist_reset @0x837494B0 — reset the playlist position so the next advance starts fresh. */

#include "headers/game_engine_playlist_s.h"

void game_engine_playlist_reset(void)
{
    game_engine_playlist.now_playing = -1;
}
