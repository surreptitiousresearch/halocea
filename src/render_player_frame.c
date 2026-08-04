/* render_player_frame @0x83? — set up and submit the render for one player's window. The camera's structure
 * visibility is resolved, atmospheric and planar fog are gathered and used to pull the far clip plane in (so
 * fully-dense fog bounds the view distance), and the near/far planes are sanitised. The frustum bounds are
 * built and, when a tiled super-sampled screenshot is in progress, narrowed to the current tile. Frustums are
 * built for both the render and rasterizer cameras. In single-window mode a reflective mirror surface, if
 * found and supported, is rendered first as a separate reflection pass. Finally the main window is rendered.
 *
 * DEVIATION 1: the screenshot-tiling block converted tile indices to float via hardware fcfid, which the
 * decompiler rendered as __int64/OVERLAPPED word arithmetic; restored as plain (float) casts subdividing the
 * frustum-bounds rectangle into a (size*count) grid and selecting tile (x, count-1-y).
 * DEVIATION 2: the original names both a function and a struct "render_window" (a tag-vs-function distinction
 * collapsed by our typedef); the function is declared below and the window struct uses the same name. */

/* Use the struct TAG header (no typedef) so the render_window() function
 * declaration below does not collide with a `render_window` typedef. */
#include <stdint.h>
#include "headers/render_window_struct.h"
#include "headers/render_globals.h"
#include "headers/render_camera.h"
#include "headers/render_frustum.h"
#include "headers/render_mirror.h"
#include "headers/real_rectangle2d.h"
#include "headers/point2d.h"
#include "headers/rasterizer_globals.h"
#include "headers/render_planar_fog_mode.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/render_fog.h"
#include "headers/render_fog.h"
extern void structure_visibility_find_camera(const render_camera *camera);
extern void scenario_get_atmospheric_fog(int16_t local_player_index, int16_t sky_index, real_point3d *camera_point, render_fog *render_fog);
extern void structure_get_planar_fog(int16_t cluster_index, render_fog *fog);
extern void render_camera_build_frustum_bounds(const render_camera *camera, real_rectangle2d *frustum_bounds);
extern void render_camera_build_frustum(const render_camera *camera, const real_rectangle2d *frustum_bounds, render_frustum *frustum, uint8_t build_projection);
extern int16_t main_get_window_count(void);
extern uint8_t structure_visibility_find_mirror(const render_camera *camera, const render_frustum *frustum, render_mirror *result);
extern void render_camera_mirror(const render_camera *camera, const render_mirror *mirror, render_camera *result);
extern void rasterizer_profile_enable(uint8_t enable);
extern void render_window(int16_t local_player_index, const render_camera *source_camera, const render_frustum *source_frustum, const render_camera *rasterizer_camera, const render_frustum *rasterizer_frustum, int16_t rasterizer_target, uint8_t has_mirror);

void render_player_frame(struct render_window *window, const point2d *screenshot_combined_index)
{
    render_camera *camera = &window->render_camera;
    unsigned __int8 has_mirror = 0;

    structure_visibility_find_camera(&window->render_camera);
    render.fog.runtime_flags = 0;
    scenario_get_atmospheric_fog(window->local_player_index, render.visible_sky_index, &camera->position,
                                 &render.fog);
    structure_get_planar_fog(render.cluster_index, &render.fog);

    float atmospheric_maximum_distance = render.fog.atmospheric_maximum_distance;
    float planar_maximum_distance = render.fog.planar_maximum_distance;
    if (atmospheric_maximum_distance != 0.0f && (unsigned __int16)render.visible_sky_index == 0xFFFF)
    {
        if (render.fog.planar_maximum_distance > atmospheric_maximum_distance)
        {
            render.fog.planar_maximum_distance = render.fog.atmospheric_maximum_distance;
            planar_maximum_distance = render.fog.atmospheric_maximum_distance;
        }
    }

    if (render.fog.atmospheric_maximum_density == 1.0f && atmospheric_maximum_distance != 0.0f)
    {
        if (window->render_camera.z_far <= atmospheric_maximum_distance)
            atmospheric_maximum_distance = window->render_camera.z_far;
        window->render_camera.z_far = atmospheric_maximum_distance;
        planar_maximum_distance = render.fog.planar_maximum_distance;
    }

    if (render.fog.planar_mode == _render_planar_fog_mode_fully_fogged && planar_maximum_distance != 0.0f)
    {
        if (window->render_camera.z_far <= planar_maximum_distance)
            planar_maximum_distance = window->render_camera.z_far;
        window->render_camera.z_far = planar_maximum_distance;
    }

    float z_near = window->render_camera.z_near;
    if (window->render_camera.z_far <= z_near)
    {
        if (!warned)
            warned = 1;
        window->render_camera.z_far = z_near + 0.0099999998f;
    }

    real_rectangle2d frustum_bounds;
    render_camera_build_frustum_bounds(camera, &frustum_bounds);

    int tile_count = global_screenshot_size * global_screenshot_count;
    if (screenshot_combined_index && tile_count > 0)
    {
        float tile_width = (frustum_bounds.n[1] - frustum_bounds.n[0]) / (float)tile_count;
        float tile_height = (frustum_bounds.n[3] - frustum_bounds.n[2]) / (float)tile_count;
        int column = screenshot_combined_index->__s1.x;
        int row = tile_count - screenshot_combined_index->__s1.y - 1;
        frustum_bounds.n[2] = (float)row * tile_height + frustum_bounds.n[2];
        frustum_bounds.n[0] = (float)column * tile_width + frustum_bounds.n[0];
        frustum_bounds.n[1] = frustum_bounds.n[0] + tile_width;
        frustum_bounds.n[3] = frustum_bounds.n[2] + tile_height;
    }

    render_frustum frustum;
    render_frustum rasterizer_frustum;
    render_camera_build_frustum(camera, &frustum_bounds, &frustum, 1);
    render_camera_build_frustum(&window->rasterizer_camera, &frustum_bounds, &rasterizer_frustum, 1);

    if (main_get_window_count() == 1)
    {
        render_mirror mirror;
        if (structure_visibility_find_mirror(camera, &frustum, &mirror)
            && !rasterizer_globals.alpha_render_targets_disabled)
        {
            __int16 saved_cluster_index = render.cluster_index;
            render_camera mirror_camera;
            render_frustum mirror_frustum;
            render_camera_mirror(camera, &mirror, &mirror_camera);
            render_camera_build_frustum(&mirror_camera, &frustum_bounds, &mirror_frustum, 1);
            rasterizer_profile_enable(0);
            render.cluster_index = mirror.cluster_index;
            render_window(-1, &mirror_camera, &mirror_frustum, &mirror_camera, &mirror_frustum, 2, has_mirror /* still 0: mirror pass has no nested mirror */);
            render.cluster_index = saved_cluster_index;
            rasterizer_profile_enable(1);
            has_mirror = 1;
        }
    }

    render_window(window->local_player_index, camera, &frustum, &window->rasterizer_camera, &rasterizer_frustum,
                  1, has_mirror);
}
