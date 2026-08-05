/* game_unload @0x83687AB4 — tear down the currently loaded map. If a precache is still streaming, raise
 * the loading-progress widget and pump pregame render frames until the precache reports done (status!=0),
 * closing all widgets afterward and showing the damaged-media error if it failed (status 2). Finally, if a
 * map is loaded, unload the scenario, reset the debug random seed, and clear the map-loaded flag.
 * DEVIATION: main_pregame_render / main_rasterizer_throttle / main_present_frame are independent no-arg
 * calls (threaded-r3 artifact), not a nested call chain. */

#include <stdint.h>
#include "headers/game_globals.h"

extern uint8_t cache_files_precache_in_progress(void);
extern void ui_widget_load_progress_widget(void);
extern int16_t cache_files_precache_map_status(float *progress);
extern void main_pregame_render(void);
extern void main_rasterizer_throttle(void);
extern void main_present_frame(void);
extern void ui_widgets_close_all(void);
extern void display_error_damaged_media(void);
extern void cache_files_precache_map_end(void);
extern void scenario_unload(void);
extern void random_seed_debug_log(uint8_t enable);

void game_unload(void)
{
    if ( cache_files_precache_in_progress() )
    {
        game_globals->map_load_in_progress = 1;
        ui_widget_load_progress_widget();
        int16_t status;
        do
        {
            status = cache_files_precache_map_status(&game_globals->loading_progress);
            main_pregame_render();
            main_rasterizer_throttle();
            main_present_frame();
        }
        while ( !status );
        ui_widgets_close_all();
        if ( status == 2 )
            display_error_damaged_media();
        cache_files_precache_map_end();
    }
    if ( game_globals->map_loaded )
    {
        scenario_unload();
        random_seed_debug_log(0);
        game_globals->map_loaded = 0;
    }
}
