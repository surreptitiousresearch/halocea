/* rasterizer_lens_flares_submit_occlusion_tests @0x837928A8 — for each lens flare belonging to the current
 * window, submit a hardware occlusion query at a point near the light and stash the resulting visible-pixel
 * count back on the flare. The query point is offset from the flare position per the definition's
 * occlusion_offset_direction: 0 pulls back along the camera forward by the occlusion radius, 1 pushes out
 * along the flare direction by radius*sqrt(2), 2 uses the position unchanged. Skipped while taking a multi-
 * tile screenshot, off the main render target, or with no flares. */

#include <stdint.h>
#include "headers/render_widget_type.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/lens_flare_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/rasterizer_target.h"
#include "headers/lens_flare_occlusion_offset_direction.h"
#include "headers/blam_data_globals.h"


extern void rasterizer_widget_begin(int16_t type, uint16_t flags);
extern void rasterizer_widget_end(void);
/* DB prototype declares void; the call site stores r3 back into the flare, so it returns the pixel count. */
extern int rasterizer_widget_submit_occlusion_test(const real_point3d *point, float radius, int index);
extern real_vector3d * uncompress_int32_to_real_vector3d(real_vector3d *result, unsigned int i);

void rasterizer_lens_flares_submit_occlusion_tests(void)
{
    if (!rasterizer_debug_options.draw_lens_flares)
        return;

    int taking_screenshot = global_screenshot_count > 1
        || (global_screenshot_count == 1 && global_screenshot_size > 1);
    if (taking_screenshot || global_window_parameters.rasterizer_target != _rasterizer_target_render_primary || local_lens_flare_count <= 0)
        return;

    rasterizer_widget_begin(_widget_type_internal_occlusion_test, 1);

    for (int i = 0; i < local_lens_flare_count; i = (int16_t)(i + 1))
    {
        rasterizer_lens_flare_submit_parameters *params = &local_lens_flare_parameters[i];
        lens_flare_definition *definition = params->definition;

        real_vector3d direction;
        uncompress_int32_to_real_vector3d(&direction, params->compressed_direction);

        if ((params->compressed_window_index & 0x7F) != global_window_parameters.window_index)
            continue;

        int16_t offset_direction = definition->occlusion_offset_direction;
        float occlusion_radius = definition->occlusion_radius;

        real_point3d occlusion_point;
        if (offset_direction == _lens_flare_occlusion_offset_direction_toward_viewer)
        {
            occlusion_point.n[0] = global_window_parameters.camera.forward.n[0] * -occlusion_radius + params->position.n[0];
            occlusion_point.n[1] = global_window_parameters.camera.forward.n[1] * -occlusion_radius + params->position.n[1];
            occlusion_point.n[2] = global_window_parameters.camera.forward.n[2] * -occlusion_radius + params->position.n[2];
        }
        else if (offset_direction == _lens_flare_occlusion_offset_direction_marker_forward)
        {
            float scaled_radius = occlusion_radius * 1.4142135f;
            occlusion_point.n[0] = direction.n[0] * scaled_radius + params->position.n[0];
            occlusion_point.n[1] = direction.n[1] * scaled_radius + params->position.n[1];
            occlusion_point.n[2] = direction.n[2] * scaled_radius + params->position.n[2];
        }
        else if (offset_direction < NUMBER_OF_LENS_FLARE_OCCLUSION_OFFSET_DIRECTIONS)   /* == _none */
        {
            occlusion_point = params->position;
        }
        /* offset_direction >= NUMBER_OF_LENS_FLARE_OCCLUSION_OFFSET_DIRECTIONS leaves occlusion_point uninitialized, matching the original */

        params->internal__occlusion_pixels =
            rasterizer_widget_submit_occlusion_test(&occlusion_point, occlusion_radius, i);
    }

    rasterizer_widget_end();
}
