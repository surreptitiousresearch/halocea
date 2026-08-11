/* game_engine_playlist_begin @0x8374D1A0 — starts the multiplayer playlist from the top: resets the
 * now-playing cursor, advances to the first stage, applies that stage's map name and game variant, and
 * if no network game is active resets the map locally.
 *
 * DEVIATION: the decompiler threaded network_game_is_active()'s r3 out as a return value. The binary
 * defines no r3 of its own on either exit path and none of the 3 callers consumes r3 — attested void. */

/* blam_data_globals.h first: it supplies wchar_t before game_variant.h is parsed (migration) */
#include <stdint.h>
#include "headers/blam_data_globals.h"
#include "headers/game_engine_playlist_s.h"
#include "headers/play_stage.h"


extern void game_engine_playlist_next(void);
extern void main_set_multiplayer_map_name(const char *name);
extern void game_set_game_variant(game_variant *variant);
extern uint8_t network_game_is_active(void);
extern void main_reset_map(void);

void game_engine_playlist_begin(void)
{
    game_engine_playlist.now_playing = -1;
    game_engine_playlist_next();
    main_set_multiplayer_map_name(global_stage.map_name);
    game_set_game_variant(&global_stage.game_variant);

    if (!network_game_is_active())
        main_reset_map();
}
