/* decal_sprite_get_bounds @0x837401C0 — resolve a decal's bitmap-group sequence/sprite record and compute
 * two rectangles: `sprite_bounds` (the raw UV bounds copied straight out of the bitmap group tag) and
 * `extent` (the world-space quad extent scaled by `radius`/runtime_maximum_sprite_extent and, unless flag
 * 0x100 is set, the sprite's own aspect-corrected width/height).
 *
 * DEVIATION: Hex-Rays flagged "local variable allocation has failed" and fabricated a phantom 7th
 * parameter (`a7`); the DB's own prototype confirms only 6 (and `radius` is `float`, not the `double` the
 * raw decompile showed). disasm_range(0x837401C0,0x83740360) resolves the whole function cleanly: it also
 * revealed the decompiler had swapped which output pointer is which — the values it labeled `extent`
 * (copied straight from the sprite's bounds, untouched) are actually written through the 5th parameter
 * (sprite_bounds per the DB prototype), and the scaled/computed rectangle it never named goes through the
 * 6th (extent). New headers: decal_definition.h, shader_decal.h. */

#include "headers/decal_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_data.h"
#include "headers/real_rectangle2d.h"
#include "headers/decal_definition_flags.h"
#include <stdint.h>

void decal_sprite_get_bounds(const decal_definition *definition, int16_t sequence_index, int16_t sprite_index,
        float radius, real_rectangle2d *sprite_bounds, real_rectangle2d *extent)
{
    bitmap_group *bitmap_group_definition = TAG_GET(bitmap_group, definition->shader.decal.map.index);

    bitmap_group_sequence *sequence =
        &((bitmap_group_sequence *)bitmap_group_definition->sequences.address)[sequence_index];
    /* recovered: (char *)sprites.address + 32 * sprite_index -> typed [sprite_index] (stride 0x20) */
    bitmap_group_sprite *sprite =
        (bitmap_group_sprite *)sequence->sprites.address + sprite_index;

    *sprite_bounds = sprite->bounds;

    bitmap_data *bitmap =
        &((bitmap_data *)bitmap_group_definition->bitmaps.address)[sprite->bitmap_index];

    float aspect_scale = 1.0f;
    if ( (definition->flags & (1u << _decal_definition_preserve_aspect_bit)) != 0 )
    {
        aspect_scale = ((sprite->bounds.x1 - sprite->bounds.x0) / (sprite->bounds.y1 - sprite->bounds.y0))
                     * ((float)bitmap->height / (float)bitmap->width);
    }

    float scale = radius / definition->runtime_maximum_sprite_extent;
    float width_scale = (float)bitmap->width * scale;
    float height_scale = (float)bitmap->height * scale * aspect_scale;

    extent->x0 = -sprite->registration_point.n[0] * width_scale;
    extent->x1 = (sprite->bounds.x1 - sprite->registration_point.n[0] - sprite->bounds.x0) * width_scale;
    extent->y0 = -sprite->registration_point.n[1] * height_scale;
    extent->y1 = (sprite->bounds.y1 - sprite->registration_point.n[1] - sprite->bounds.y0) * height_scale;
}
