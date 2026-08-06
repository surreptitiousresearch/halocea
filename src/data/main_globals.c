/* main_globals @ 0x8441DFF8 (.data, 1072 bytes)
 * DB applied_types: _main_globals main_globals;
 * Image bytes (big-endian), decoded from the binary .data record:
 *   +0x00 last_time_clocks           = 0x0000000000000000
 *   +0x08 last_time_msec             = 0x00000000
 *   +0x10 last_render_clocks         = 0x0000000000000000
 *   +0x18 last_vblank_index          = 0x0000000000000000
 *   +0x20 last_initial_vblank_index  = 0x0000000000000000
 *   +0x28 last_achievable_vblank_index = 0x0000000000000000
 *   +0x30 last_present_vblank_index  = 0x0000000000000000
 *   +0x38 did_time_overflow_occur    = 0x00
 *   +0x3C seconds_elapsed            = 0x00000000 -> 0f
 *   +0x40 connection                 = 0x0000
 *   +0x42 screenshot_identifier      = 0x0000
 *   +0x44 movie                      = 0x00000000
 *   +0x48 recording_start_tick       = 0x00000000
 *   +0x4C recording_stop_tick        = 0x00000000
 *   +0x50 recording_frame_index      = 0x00000000
 *   +0x54 recording_dt               = 0x00000000 -> 0f
 *   +0x58 reset_map                  = 0x00
 *   +0x59 rename_map                 = 0x00
 *   +0x5A revert_map                 = 0x00
 *   +0x5B skip_cinematic             = 0x00
 *   +0x5C save_map                   = 0x00
 *   +0x5D save_map_safely            = 0x00
 *   +0x5E save_map_timeout           = 0x00
 *   +0x5F saving_map                 = 0x00
 *   +0x60 ticks_until_next_save_check = 0x00000000
 *   +0x64 ticks_unable_to_save       = 0x00000000
 *   +0x68 map_change_load_timer      = 0x00000000
 *   +0x6C safe_intervals             = 0x0000
 *   +0x6E won_map                    = 0x00
 *   +0x6F lost_map                   = 0x00
 *   +0x70 respawn                    = 0x00
 *   +0x71 save_core                  = 0x00
 *   +0x72 load_core                  = 0x00
 *   +0x73 load_core_at_startup       = 0x00
 *   +0x74 switch_to_structure_bsp_index = 0x0000
 *   +0x76 main_menu_scenario_loaded  = 0x00
 *   +0x77 want_to_be_at_main_menu    = 0x00
 *   +0x78 run_xdemos                 = 0x00
 *   +0x79 fade_to_dashboard          = 0x00
 *   +0x7A exit_to_dashboard          = 0x00
 *   +0x7B want_to_exit               = 0x00
 *   +0x7C idle_timeout               = 0x00000000
 *   +0x80 idle_last_interesting      = 0x00000000
 *   +0x84 idle_last_activity         = 0x00000000
 *   +0x88 playback_last_recording    = 0x00
 *   +0x89 halt_time_scale            = 0x00
 *   +0x8A restart_time               = 0x00
 *   +0x8B load_last_solo_level       = 0x00
 *   +0x8C cutscene_skip              = 0x00
 *   +0x8E skip_ticks                 = 0x0000
 *   +0x90 loss_timer                 = 0x0000
 *   +0x92 respawn_timer              = 0x0000
 *   +0x94 queue_map                  = 0x00
 *   +0x95 pad0                       = 0x000000
 *   +0x98 solo_try_and_load_from_persistent_storage = 0x00
 *   +0x99 soloplayer_map_name        = 0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x199 multiplayer_map_name       = 0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x299 queued_map_name            = 0x00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x399 want_to_connect            = 0x00
 *   +0x39A connect_address            = 0x0000000000000000000000000000000000000000000000000000000000000000
 *   +0x3BA connect_password           = 0x000000000000000000
 *   +0x3C4 vblank_interval_current    = 0x0000
 *   +0x3C6 vblank_interval_minimum    = 0x0000
 *   +0x3C8 vblank_interval_held       = 0x00
 *   +0x3CA vblank_failure_count       = 0x000000000000000000000000
 *   +0x3D8 vblank_last_failure_time   = 0x000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
 *   +0x408 vblank_flip_counter        = 0x00000000
 *   +0x40C vblank_flip_delta_next_index = 0x0000
 *   +0x40E vblank_flip_deltas         = 0x000000000000000000000000000000000000000000000000000000000000
 * top-level standalone-loop control block (data-bss, 1072 bytes).
 */
#include "../headers/main_globals.h"

struct main_globals main_globals;
