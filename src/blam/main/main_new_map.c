/* main_new_map @0x83689E18 — bring a freshly loaded map online: flush input, load the game, initialize the
 * new map (unless errors occurred), create local players, start game time and fire the initial pulse, then
 * reset all the per-map request flags. Loads a startup core if one was queued, optionally restores from
 * persistent storage, and starts closing the loading screen in single player.
 *
 * Deviation: input_abstraction_flush/create_local_players/game_time_start/game_initial_pulse are no-arg;
 * the decompiler chained them through r3. Unchained (game_time_start/game_initial_pulse return int, but
 * the return is discarded here). */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/game_options.h"

extern void input_flush(void);
extern void input_abstraction_flush(void);
extern uint8_t game_load(game_options *options);
extern void game_initialize_for_new_map(void);
extern uint8_t errors_handle(void);
extern void create_local_players(void);
extern void game_time_start(void);
extern void game_initial_pulse(void);
extern uint32_t system_milliseconds(void);
extern void game_state_try_and_load_from_persistent_storage(void);
extern void ui_widgets_disable_pause_game(int duration_ticks);
extern void ui_loading_start_closing(uint8_t why_are_we_always_preparing_just_go);

void main_new_map(game_options *options)
{
    input_flush();
    input_abstraction_flush();
    if ( game_load(options) )
        game_initialize_for_new_map();

    if ( !errors_handle() )
    {
        create_local_players();
        game_time_start();
    }
    game_initial_pulse();

    main_globals.switch_to_structure_bsp_index = -1;
    main_globals.reset_map = 0;
    main_globals.won_map = 0;
    main_globals.rename_map = 0;
    main_globals.revert_map = 0;
    main_globals.lost_map = 0;
    main_globals.skip_cinematic = 0;
    main_globals.save_map = 0;
    main_globals.respawn = 0;
    main_globals.save_core = 0;
    main_globals.load_core = main_globals.load_core_at_startup;
    main_globals.load_core_at_startup = 0;
    main_globals.idle_last_activity = system_milliseconds();

    if ( main_globals.solo_try_and_load_from_persistent_storage )
        game_state_try_and_load_from_persistent_storage();
    ui_widgets_disable_pause_game(30);
    if ( !main_globals.connection )
        ui_loading_start_closing(0);
}
