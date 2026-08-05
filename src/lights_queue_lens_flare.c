/* lights_queue_lens_flare @0x836F7B78 — queue one lens flare for the frame (up to 8), skipping fully-black
 * colors. Kept the decompiler's literal byte-packing expression for compressed_light_color (matches the
 * identical idiom already preserved as-is in bitmap_format_to_a8r8g8b8.c) rather than collapsing it — it
 * reduces to plain 0xFF000000 | R<<16 | G<<8 | B, but the raw form is the established house style here. */

#include <stdint.h>
#include "headers/lights_globals.h"
#include "headers/global_tag_instances.h"
#include "headers/render_globals.h"
#include "headers/real_rgb_color.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"


extern unsigned int compress_real_to_int8(float value);
extern unsigned int compress_real_vector3d_to_int32_clamp(const real_vector3d *v);

void lights_queue_lens_flare(int definition_index, const real_point3d *position, const real_vector3d *direction,
                              const real_vector3d *up, const real_rgb_color *color, float scale)
{
    if ( lights_globals.queued_lens_flare_count >= 8 )
        return;
    if ( color->n[0] == 0.0f && color->n[1] == 0.0f && color->n[2] == 0.0f )
        return;

    rasterizer_lens_flare_submit_parameters *flare = &lights_globals.queued_lens_flares[lights_globals.queued_lens_flare_count];

    flare->compressed_light_color = ((((uint8_t)(color->n[0] * 255.0f) << 8)
                                     | 0xFFFF0000
                                     | (uint8_t)(color->n[1] * 255.0f)) << 8)
                                   | (uint8_t)(color->n[2] * 255.0f);
    flare->compressed_light_scale = compress_real_to_int8(scale);
    flare->definition = TAG_GET(struct lens_flare_definition, definition_index);
    flare->position = *position;
    flare->compressed_direction = compress_real_vector3d_to_int32_clamp(direction);
    flare->compressed_up = compress_real_vector3d_to_int32_clamp(up);
    flare->light_index = -1;
    flare->light_identifier = -1;
    flare->compressed_window_index = render.window_index;
    flare->lens_flare_index = lights_globals.queued_lens_flare_count++;
}
