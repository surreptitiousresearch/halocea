/* game_tick @ 0x83688080 — one deterministic game-logic tick (1/30 s). This is the heart of the Blam
 * simulation: it advances every gameplay system in a fixed order so that the simulation is identical
 * across networked machines. Bracketed by the g_InsideGameTick guard.
 *
 * Network bookkeeping frames the tick: on a server (connection 2) it flushes super-remote player
 * actions up front; on a client (connection 1) it applies the server's authoritative positions before
 * simulating, then re-applies them and runs client catch-up afterwards. The server, after simulating,
 * remembers the last completed client update and pushes local player updates to the network.
 *
 * Deviation: the decompiler threaded every call's return through the next call's argument register
 * (v2..v23) — a PPC register-reuse artifact. These updates take no such argument; each is an
 * independent call. The two indirect ((int(*)(void))players_apply_server_position_to_other_clients)
 * casts are likewise just the plain call. */

#include <stdint.h>
#include "headers/game_globals.h"
#include "headers/game_connection.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern int16_t game_connection(void);
extern void player_update_server_flush_super_remote_player_actions(void);
extern void players_apply_server_position_to_other_clients(void);
extern void real_math_reset_precision(void);
extern void remove_quitting_players_from_game(void);
extern void game_allegiance_update(void);
extern void units_update(void);
extern void ai_update(void);
extern void players_update_before_game_server(void);
extern void effects_update(float dt);
extern void lock_global_random_seed(void);
extern void vibrate_update(void);
extern void first_person_weapons_update(void);
extern void unlock_global_random_seed(void);
extern void game_engine_update(void);
extern void editor_update(void);
extern void hs_update(void);
extern void recorded_animations_update(void);
extern void objects_update(void);
extern void players_update_after_game(void);
extern void hud_update(void);
extern void player_effect_update(void);
extern void players_server_catchup_on_client_updates(void);
extern void players_remember_last_completed_client_update(void);
extern void player_update_server_local_player_updates_to_network(void);
extern network_game_server *global_network_game_server_get(void);
extern void players_client_catchup_on_server_updates(void);

void game_tick(int ticks_remaining_this_frame)
{
    g_InsideGameTick = 1;

    if ( game_connection() == _game_connection_network_server )
        player_update_server_flush_super_remote_player_actions();
    if ( game_connection() == _game_connection_network_client )
        players_apply_server_position_to_other_clients();

    real_math_reset_precision();
    remove_quitting_players_from_game();
    game_allegiance_update();
    units_update();
    ai_update();
    players_update_before_game_server();

    effects_update(game_globals->players_are_double_speed ? 0.016666668 : SECONDS_PER_TICK);

    lock_global_random_seed();
    vibrate_update();
    first_person_weapons_update();
    unlock_global_random_seed();

    game_engine_update();
    editor_update();
    hs_update();
    recorded_animations_update();
    objects_update();
    players_update_after_game();
    hud_update();
    player_effect_update();

    if ( game_connection() == _game_connection_network_server )
    {
        if ( !ticks_remaining_this_frame )
            players_server_catchup_on_client_updates();
        players_remember_last_completed_client_update();
        player_update_server_local_player_updates_to_network();
        if ( game_connection() == _game_connection_network_server )
            global_network_game_server_get();
    }

    if ( game_connection() == _game_connection_network_client )
    {
        players_apply_server_position_to_other_clients();
        players_client_catchup_on_server_updates();
        players_apply_server_position_to_other_clients();
    }

    g_InsideGameTick = 0;
}
