/* Prototypes for the game-state save/load callback tables (before_load_procs,
 * before_save_procs, after_load_procs — 0x84172060..0x841720A3). Declarations match each
 * function's defining TU in src/ exactly, so the table initializers cross-reference the
 * definitions and signature drift is a compile error.
 *
 * Deviation note: the binary invokes every entry as void-no-arg and ignores r3; the int /
 * void* returns below are decompiler ABI noise from the definitions. Table storage casts
 * to game_state_proc for this reason. */
#pragma once

typedef void (*game_state_proc)(void);

/* before_save_procs (0x84172060) */
#ifdef __cplusplus
extern "C" {
#endif
extern void dummy(void);                                            /* 0x83684478 */

/* before_load_procs (0x84172064) */
extern void game_sound_clear(void);                                 /* 0x83713810 */

/* after_load_procs (0x84172068, 15 entries, table order) */
extern void scenario_reload_structure_bsp_if_necessary(void);      /* 0x83703C20 */
extern void sound_stop_all(void);                                  /* 0x83719840 */
extern void game_sound_restore(void);                              /* 0x83713898 */
extern void observer_initialize_for_new_map(void);                 /* 0x8370E8C8 */
extern void update_queues_reset_and_fill_with_lies(void);  /* 0x836F6988 */
extern void rasterizer_detail_objects_initialize_for_new_map(void);/* 0x83712F08 */
extern void rasterizer_decals_initialize_for_new_map(void);        /* 0x836A6020 */
extern void rasterizer_decals_update_function_pointers(void);      /* 0x836A6018 */
extern void recorded_animations_clear_debug_storage(void);         /* 0x83712000 */
extern void ai_debug_initialize_for_new_map(void);                 /* 0x83684470 */
extern void structure_detail_objects_flush(void);                  /* 0x83711650 */
extern void game_state_set_revert_time(void);                      /* 0x83684DA0 */
extern void player_control_fix_for_loaded_game_state(void);        /* 0x836A8890 */
extern void director_initialize_for_saved_game(void);              /* 0x836E5B08 */
extern void scripted_hud_messages_clear(void);                     /* 0x836A4AB0 */
#ifdef __cplusplus
}
#endif
