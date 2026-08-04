/* Storage definition for after_load_procs (0x84172068, 60 bytes = 15 entries).
 * Saved-game "after load" callback table, reconstructed from binary reloc order.
 *
 * Deviation: entries whose definitions carry non-void ABI-noise returns are cast to
 * game_state_proc; the binary calls each entry void-no-arg and ignores r3. */

#include "../headers/blam_data_globals.h"
#include "../headers/game_state_procs.h"

void (*after_load_procs[15])(void) = {
    (game_state_proc)scenario_reload_structure_bsp_if_necessary,
    sound_stop_all,
    (game_state_proc)game_sound_restore,
    (game_state_proc)observer_initialize_for_new_map,
    (game_state_proc)update_queues_reset_and_fill_with_lies,
    rasterizer_detail_objects_initialize_for_new_map,
    (game_state_proc)rasterizer_decals_initialize_for_new_map,
    rasterizer_decals_update_function_pointers,
    recorded_animations_clear_debug_storage,
    ai_debug_initialize_for_new_map,
    structure_detail_objects_flush,
    game_state_set_revert_time,
    player_control_fix_for_loaded_game_state,
    director_initialize_for_saved_game,
    scripted_hud_messages_clear,
};
