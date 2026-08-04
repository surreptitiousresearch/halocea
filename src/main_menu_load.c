/* main_menu_load @ 0x8368B3D8 — load the UI/main-menu scenario, reset networking and
 * game-time, and hand input back to the menu event system. */
#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/scenario.h"

#include "headers/tag_block.h"
extern void main_load_ui_scenario(uint8_t precache_resources);
extern void main_screen_shell_load(void);
extern void predicted_resources_precache(const tag_block *predicted_resources);
extern void ui_loading_start_closing(uint8_t why_are_we_always_preparing_just_go);
extern void hud_chat_box_clear(void);
extern void trouble_is_brewing_dismissed(void);
extern void update_server_delete(void);
extern uint8_t update_server_new(void);
extern void update_server_start(void);
extern void game_time_dispose_from_old_map(void);
extern void game_time_initialize_for_new_map(void);
extern void game_time_start(void);
extern void hs_runtime_dispose_from_old_map(void);
extern void hs_runtime_initialize_for_new_map(void);
extern void input_abstraction_set_enabled_mode_events(uint8_t enabled);

void main_menu_load(void)
{
    if ( !main_globals.main_menu_scenario_loaded )
        main_load_ui_scenario(0);
    main_screen_shell_load();
    if ( global_scenario )
        predicted_resources_precache(&global_scenario->predicted_ui_resources);
    ui_loading_start_closing(1);
    hud_chat_box_clear();
    /* The following int(int) callees have no argument set up in the disasm and
     * their returns are never consumed (main_menu_load returns void): the reused
     * r3 register was decompiler-threaded as a bogus `s` local. Straight-line
     * calls; the placeholder 0 feeds the stale int params (really void(void)). */
    trouble_is_brewing_dismissed();
    update_server_delete();
    update_server_new();
    update_server_start();
    game_time_dispose_from_old_map();
    game_time_initialize_for_new_map();
    game_time_start();
    hs_runtime_dispose_from_old_map();
    hs_runtime_initialize_for_new_map();
    main_globals.want_to_be_at_main_menu = 0;
    input_abstraction_set_enabled_mode_events(1);
}
