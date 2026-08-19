/* game_engine_update @ 0x83753000 — per-tick multiplayer game-mode update (no-op in singleplayer where
 * game_engine is null). Updates multiplayer sound, purges expired state and weapons; (re)spawns items
 * on the server / singleplayer / saved-film; and on the server replicates score and state to the network.
 *
 * Then per player: unless the variant suppresses it, the player's unit camera-shake accumulators are
 * cleared; per-player always-invisible and teleporter handling run; and the mode's player_update hook
 * fires. After the player loop the mode's global update hook fires.
 *
 * Finally drives the end-game state machine: in post-game delay it fades ambience down over the last
 * ~1 s (mute everything, then restore ambient classes at 0.2 over 30 ticks), counts the ending timer
 * down, and on the server advances to the rasterize-delay mode; outside post-game it ends the game when
 * the mode says it should.
 *
 * Deviation: the decompiler threaded the early update calls' returns through the next argument and
 * dropped the constant 30-tick fade duration on the sound_class_set_gain calls (r5 = 0x1E); both
 * restored. The first gain call mutes all classes (empty substring ""). */

#include <stdint.h>
#include "headers/game_engine.h"
#include "headers/game_engine_flags.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/game_engine_globals.h"
#include "headers/game_globals.h"
#include "headers/data_iterator.h"
#include "headers/game_connection.h"
#include "headers/game_engine_universal_variant_flags.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int16_t game_connection(void);
extern void game_engine_update_multiplayer_sound(void);
extern void game_engine_update_purge(void);
extern void game_engine_update_weapons(void);
extern void game_engine_update_item_spawn(void);
extern void game_engine_replicate_score_and_state_to_network(message_delta_processor_mode mode, int machine_index);
extern void game_engine_update_player_always_invis(int player_index);
extern void game_engine_update_teleporter(int player_index);
extern void sound_class_set_gain(const char *substring, float gain, int16_t ticks);
extern void game_engine_progress_to_postgame_rasterize_delay(void);
extern void game_engine_change_mode_to_network(game_engine_mode new_mode);
extern network_game_server *global_network_game_server_get(void);
extern uint8_t game_engine_should_end_game(void);
extern void game_engine_end_game(void);

extern void game_engine_update_player_no_shield(int player_index);
void game_engine_update(void)
{
    data_iterator iter;

    if ( !game_engine )
        return;

    game_engine_update_multiplayer_sound();
    game_engine_update_purge();
    game_engine_update_weapons();

    if ( game_connection() == _game_connection_network_server || game_connection() == _game_connection_local || game_connection() == _game_connection_film_playback )
        game_engine_update_item_spawn();
    if ( game_connection() == _game_connection_network_server )
        game_engine_replicate_score_and_state_to_network(_message_delta_mode_incremental, -1);

    data_iterator_new(&iter, player_data);
    while ( data_iterator_next(&iter) )
    {
        /* DEVIATION: collapsed verbatim-inlined copy of game_engine_update_player_no_shield@0x8374D208 (zero-xref donor); disasm-confirmed field-for-field match (shift=3, unit_index@+52, shield_vitality@+220, maximum_shield_vitality@+228). Loop var iter.index passed live; donor takes no NULL-guarded arg to fold. */
        int player_index = iter.index;
        game_engine_update_player_no_shield(iter.index);

        game_engine_update_player_always_invis(player_index);
        game_engine_update_teleporter(iter.index);
        if ( game_engine->player_update )
            game_engine->player_update(iter.index);
    }

    if ( game_engine->update )
        game_engine->update();

    if ( game_engine_globals.mode )
    {
        if ( game_engine_globals.mode == game_engine_mode_postgame_delay )
        {
            float ending_timer = game_engine_globals.game_ending_timer;
            if ( game_engine_globals.game_ending_timer <= 2.0f && (game_engine_globals.flags & (1u << _game_engine_game_over_sound_disabled_bit)) == 0 )
            {
                sound_class_set_gain("", 0.0f, 30);          /* mute everything */
                sound_class_set_gain("ambient_nature", 0.2f, 30);
                sound_class_set_gain("ambient_machinery", 0.2f, 30);
                sound_class_set_gain("ambient_computers", 0.2f, 30);
                ending_timer = game_engine_globals.game_ending_timer;
                game_engine_globals.flags |= (1u << _game_engine_game_over_sound_disabled_bit);
            }
            ending_timer = (ending_timer - SECONDS_PER_TICK);
            game_engine_globals.game_ending_timer = ending_timer;
            if ( ending_timer <= 0.0f && game_connection() == _game_connection_network_server )
            {
                game_engine_progress_to_postgame_rasterize_delay();
                game_engine_change_mode_to_network(game_engine_mode_postgame_rasterize_delay);
                global_network_game_server_get();
            }
        }
    }
    else if ( game_engine_should_end_game() )
    {
        game_engine_end_game();
    }
}
