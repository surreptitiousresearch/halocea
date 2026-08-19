/* build_sprites_begin @0x837EC340 — initialize a build_sprite_data batch for a new sprite-building pass. */

#include <stdint.h>
#include "headers/build_sprite_data.h"
#include "headers/build_sprites_internal_flags.h"
#include "headers/blam_data_globals.h"


void build_sprites_begin(build_sprite_data *data, int16_t maximum_sprite_count, int bitmap_group_index, const shader_effect *shader, unsigned int flags)
{
    data->flags = flags;
    data->bitmap_group_index = bitmap_group_index;
    data->maximum_sprite_count = maximum_sprite_count;
    data->group_count = 0;
    data->sprite_count = 0;
    data->shader = shader;
    data->centroid = *global_origin3d;
    data->flags = flags | (1u << _build_sprites_valid_bit);
}
