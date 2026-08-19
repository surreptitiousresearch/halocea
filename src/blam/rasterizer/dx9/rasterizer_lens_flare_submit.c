/* rasterizer_lens_flare_submit @0x83792438 — queue one lens flare for this frame's render. Rejects the submit
 * unless lens-flare drawing is enabled, no multi-tile screenshot is in progress, and the render target is the
 * primary window. Culls flares beyond the definition's far-fade plane and fully-transparent flares. Accepted
 * flares are appended to local_lens_flare_parameters (capped at 1024, with a one-shot overflow warning). The
 * stored light/lens indices are then repacked: a flare with no light identifier encodes its light+lens indices
 * into the index fields (top bit = dynamic light); a flare that carries a light identifier resets its
 * occlusion-test row whenever that identifier changed.
 *
 * DEVIATION: the accepted-parameters store is a 40-byte struct copy `*slot = *parameters` (Hex-Rays rendered
 * it as a 10-iteration pointer-walk). The occlusion pool follows the codebase's flat unsigned char[896][18]
 * model (as in lens_flare_occlusion_test_results_get.c), not Hex-Rays' invented
 * lens_flare_occlusion_test_results struct; each 18-byte row is [light_identifier:i16][8 x i16 window slots].
 * Disasm 0x8379258C / 0x837925B8 verified the index repacking (lens index +8, light index | 0x8000) and the
 * 8-halfword row reset. The camera-distance dot product keeps the decompiler's (z + (x + y)) association. */

#include <stdint.h>
#include "headers/rasterizer_lens_flare_submit_parameters.h"  /* local_lens_flare_parameters[], local_lens_flare_count */
#include "headers/lens_flare_definition.h"
#include "headers/rasterizer_debug_options_struct.h"
#include "headers/rasterizer_window_begin_parameters.h"
#include "headers/rasterizer_target.h"
#include "headers/blam_data_globals.h"


void rasterizer_lens_flare_submit(const rasterizer_lens_flare_submit_parameters *parameters)
{
    if ( !rasterizer_debug_options.draw_lens_flares )
        return;

    int screenshot_in_progress = global_screenshot_count > 1
        || (global_screenshot_count == 1 && global_screenshot_size > 1);
    if ( screenshot_in_progress || global_window_parameters.rasterizer_target != _rasterizer_target_render_primary )
        return;

    if ( local_lens_flare_count >= 1024 )
    {
        if ( !warned_6 )
            warned_6 = 1;
        return;
    }

    float far_fade_distance = parameters->definition->far_fade_distance;
    float camera_distance =
          global_window_parameters.camera.forward.n[2]
              * (parameters->position.n[2] - global_window_parameters.camera.position.n[2])
        + (global_window_parameters.camera.forward.n[0]
              * (parameters->position.n[0] - global_window_parameters.camera.position.n[0])
         + global_window_parameters.camera.forward.n[1]
              * (parameters->position.n[1] - global_window_parameters.camera.position.n[1]));
    if ( (far_fade_distance != 0.0f && camera_distance >= far_fade_distance)
        || (parameters->compressed_light_color & 0xFF000000) == 0 )
        return;

    rasterizer_lens_flare_submit_parameters *slot = &local_lens_flare_parameters[local_lens_flare_count];
    ++local_lens_flare_count;
    *slot = *parameters;

    if ( parameters->light_identifier == -1 )
    {
        if ( parameters->light_index == -1 )
        {
            slot->light_index = (int16_t)0x8000;
        }
        else
        {
            int packed = parameters->lens_flare_index | (parameters->light_index << 16);
            slot->lens_flare_index = packed + 8;
            slot->light_index = (packed >> 16) | 0x8000;
        }
    }
    else
    {
        unsigned char *occlusion_row = ((unsigned char (*)[18])local_lens_flare_occlusion_test_results)[slot->light_index];
        if ( parameters->light_identifier != *(int16_t *)occlusion_row )
        {
            /* flat [i16 light_identifier][8 x i16 window] row; clear the 8 window slots */
            for ( int i = 0; i < 8; i++ )
                ((int16_t *)(occlusion_row + 2))[i] = 0;
            *(int16_t *)occlusion_row = slot->light_identifier;
        }
    }
}
