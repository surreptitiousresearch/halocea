/* _rasterizer_frame_begin @0x8369BD60 — platform rasterizer frame-begin implementation
 * (called through the rasterizer_frame_begin thunk). Latches near-clip + frame timing,
 * starts the per-frame subsystems, and refreshes environment decals when enabled. */

#include "headers/rasterizer_globals.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_frame_begin_parameters.h"
#include "headers/blam_data_globals.h"


extern float rasterizer_get_near_clip_distance(void);
extern void rasterizer_frame_statistics_begin(void);
extern void rasterizer_water_set_visibility_for_frame(int visibility);
extern void rasterizer_lights_begin_for_new_frame(void);
extern void texture_cache_idle(void);
extern void rasterizer_decal_vertices_begin_update(void);
extern void decals_update(void);
extern void rasterizer_decal_vertices_end_update(void);

void _rasterizer_frame_begin(const rasterizer_frame_begin_parameters *parameters)
{
    rasterizer_globals.z_near = rasterizer_get_near_clip_distance();

    /* 16-byte long double copy of the frame timing block (decompiler renders it as two
     * _QWORD stores). */
    global_frame_parameters.game_time_sec = parameters->game_time_sec;

    rasterizer_frame_statistics_begin();
    rasterizer_water_set_visibility_for_frame(0);

    /* Threaded-r3 artifact: independent no-arg calls, not a nested chain. */
    rasterizer_lights_begin_for_new_frame();
    texture_cache_idle();

    if (rasterizer_debug_options.draw_environment_decals)
    {
        rasterizer_decal_vertices_begin_update();
        decals_update();
        rasterizer_decal_vertices_end_update();
    }
}
