/* rasterizer_lights_begin_for_new_frame @0x8379226C — folds this frame's lens-flare occlusion query
 * results into each flare's smoothed visibility byte, then resets the per-frame flare list. The new
 * per-flare visibility (0..255) is the fraction of the flare's pixels that passed the occlusion test;
 * it is blended toward the previous value (fading in slowly — average — and out faster — 3:1) so flares
 * don't pop. Skipped entirely when lens flares are disabled or while a multi-tile screenshot is in
 * progress.
 *
 * DEVIATION: the decompiler caches local_lens_flare_count in v4 but only refreshes it inside the
 * occlusion>0 branch (leaving it stale on the loop guard if the first flare has no pixels); the count
 * is invariant across the loop, so it is read directly here. */

#include <stdint.h>
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_lens_flare_submit_parameters.h"
#include "headers/blam_data_globals.h"


extern uint8_t *lens_flare_occlusion_test_results_get(const rasterizer_lens_flare_submit_parameters *lens_flare_parameters);
extern int rasterizer_widget_get_occlusion_test_result(int index);

void rasterizer_lights_begin_for_new_frame(void)
{
    if (!rasterizer_debug_options.draw_lens_flares)
        return;
    if (global_screenshot_count > 1 || (global_screenshot_count == 1 && global_screenshot_size > 1))
        return;
    if (local_lens_flare_count <= 0)
        return;

    int i = 0;
    do
    {
        rasterizer_lens_flare_submit_parameters *flare = &local_lens_flare_parameters[i];
        unsigned char *visibility = lens_flare_occlusion_test_results_get(flare);

        int new_visibility;
        if (flare->internal__occlusion_pixels <= 0)
        {
            new_visibility = 0;
        }
        else
        {
            new_visibility = (255 * rasterizer_widget_get_occlusion_test_result(i)
                              + (flare->internal__occlusion_pixels >> 1))
                           / flare->internal__occlusion_pixels;
            if (new_visibility >= 255)
                new_visibility = 255;
        }

        unsigned int latest = (unsigned __int8)new_visibility;
        if ((unsigned __int8)new_visibility)
        {
            unsigned int previous = *visibility;
            if (latest <= previous)
            {
                if (latest < previous)
                    *visibility = (previous + latest) / 2;   /* fade out: average */
            }
            else
            {
                *visibility = (3 * previous + latest) / 4;   /* fade in: 3:1 weighted */
            }
        }
        else
        {
            *visibility = 0;
        }

        i = (__int16)(i + 1);
    } while (i < local_lens_flare_count);

    local_lens_flare_count = 0;
}
