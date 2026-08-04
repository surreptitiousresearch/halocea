/* rasterizer_initialize_for_new_map @0x837240F8 — per-map rasterizer reset: cache the scenario's rasterizer
 * config block, reset lens-flare state, flush the text glyph cache, reset screen effects, clear the model
 * ambient reflection tint, and zero the near clip distance. */

#include "headers/game_globals_tag.h"
#include "headers/game_globals_rasterizer_data.h"
#include "headers/real_argb_color.h"
#include "headers/blam_data_globals.h"

extern real_argb_color *global_rasterizer_model_ambient_reflection_tint;

extern void rasterizer_lights_reset_for_new_map(void);
extern void rasterizer_text_cache_flush(void);
extern void rasterizer_screen_effects_initialize_for_new_map(void);
extern void rasterizer_set_near_clip_distance(float near_clip_distance);

void rasterizer_initialize_for_new_map(void)
{
    if ( global_game_globals->rasterizer_data.count )
        global_rasterizer_data = (game_globals_rasterizer_data *)global_game_globals->rasterizer_data.address;
    else
        global_rasterizer_data = 0;

    rasterizer_lights_reset_for_new_map();
    rasterizer_text_cache_flush();
    rasterizer_screen_effects_initialize_for_new_map();

    real_argb_color *tint = global_rasterizer_model_ambient_reflection_tint;
    if ( tint )
    {
        tint->n[0] = 0.0f;
        tint->n[1] = 0.0f;
        tint->n[2] = 0.0f;
        tint->n[3] = 0.0f;
    }
    rasterizer_set_near_clip_distance(0.0);
}
