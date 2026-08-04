/* main_load_ui_scenario @0x8368B2F0 — load the UI (main menu) scenario: precache levels\ui\ui, tear down
 * the previous map/game/engine, clear the game variant, bring the UI map online, set up the menu's scripted
 * camera, mark the menu active, and optionally precache the menu's predicted resources.
 *
 * Deviation: game_dispose_from_old_map/game_unload/game_engine_dispose are void no-arg; the decompiler
 * chained them through r3. Unchained. */

#include <stdint.h>
#include "headers/main_globals.h"
#include "headers/game_options.h"
#include "headers/scenario.h"

#include "headers/tag_block.h"
#include "headers/game_variant.h"
extern void game_precache_new_map(const char *map_name, uint8_t blocking);
extern void game_options_new(game_options *options);
extern char *strncpy(char *, const char *, unsigned int);
extern void game_dispose_from_old_map(void);
extern void game_unload(void);
extern void game_engine_dispose(void);
extern void game_set_game_variant(game_variant *variant);
extern void main_new_map(game_options *options);
extern void director_script_camera(uint8_t enabled);
extern void scripted_camera_set(int16_t camera_point_index, int16_t tick_count, int relative_to_object_index);
extern void main_menu_active(uint8_t active);
extern void predicted_resources_precache(const tag_block *predicted_resources);

void main_load_ui_scenario(uint8_t precache_resources)
{
    game_options options;
    game_precache_new_map("levels\\ui\\ui", 1u);
    game_options_new(&options);
    strncpy(options.map_name, "levels\\ui\\ui", 0xFFu);
    options.map_name[255] = 0;
    game_precache_new_map(options.map_name, 1u);
    game_dispose_from_old_map();
    game_unload();
    game_engine_dispose();
    game_set_game_variant(0);
    main_globals.main_menu_scenario_loaded = 1;
    main_new_map(&options);
    director_script_camera(1u);
    scripted_camera_set(0, 0, -1);
    main_menu_active(1u);
    main_globals.load_last_solo_level = 1;
    if ( precache_resources && global_scenario )
        predicted_resources_precache(&global_scenario->predicted_ui_resources);
}
