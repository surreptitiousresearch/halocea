/* after_load_procs @ 0x84172068 (.data, 60 bytes)
 * DB applied_types: void (__fastcall *after_load_procs[15])();
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x0000 = 0x83703C20 -> scenario_reload_structure_bsp_if_necessary
 *   +0x0004 = 0x83719840 -> sound_stop_all
 *   +0x0008 = 0x83713898 -> game_sound_restore
 *   +0x000C = 0x8370E8C8 -> observer_initialize_for_new_map
 *   +0x0010 = 0x836F6988 -> update_queues_reset_and_fill_with_lies
 *   +0x0014 = 0x83712F08 -> rasterizer_detail_objects_initialize_for_new_map
 *   +0x0018 = 0x836A6020 -> rasterizer_decals_initialize_for_new_map
 *   +0x001C = 0x836A6018 -> rasterizer_decals_update_function_pointers
 *   +0x0020 = 0x83712000 -> recorded_animations_clear_debug_storage
 *   +0x0024 = 0x83684470 -> ai_debug_initialize_for_new_map
 *   +0x0028 = 0x83711650 -> structure_detail_objects_flush
 *   +0x002C = 0x83684DA0 -> game_state_set_revert_time
 *   +0x0030 = 0x836A8890 -> player_control_fix_for_loaded_game_state
 *   +0x0034 = 0x836E5B08 -> director_initialize_for_saved_game
 *   +0x0038 = 0x836A4AB0 -> scripted_hud_messages_clear
 * Storage definition for after_load_procs (0x84172068, 60 bytes = 15 entries).
 * Saved-game "after load" callback table, reconstructed from binary reloc order.
 * Deviation: entries whose definitions carry non-void ABI-noise returns are cast to
 * game_state_proc; the binary calls each entry void-no-arg and ignores r3.
 */
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
