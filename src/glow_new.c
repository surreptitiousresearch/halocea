/* glow_new @0x8380B4B0 — allocate a glow instance for a glow definition: when the definition's bitmap tag
 * (definition dword +336) is a sprite group (bitmap group type word == 3), seed the new datum with the
 * definition index, its particle-count word (definition +32), and the sprite width in texels
 * ((sprite right - left) * bitmap width, from the first sequence's first sprite: sequences block +88,
 * sprites address +56, sprite bounds floats +8/+12). Returns the datum index, or NONE. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_globals.h"
#include "headers/glow_datum.h"
#include "headers/glow_definition.h"
#include "headers/bitmap_data.h"
#include "headers/bitmap_group.h"
#include "headers/bitmap_group_sequence.h"
#include "headers/bitmap_group_sprite.h"
#include "headers/bitmap_group_type.h"
#include "headers/blam_data_globals.h"


extern int datum_new(data_array *data);
extern void *datum_get(data_array *data, int index);
extern bitmap_data *bitmap_group_get_bitmap_from_sequence(int bitmap_group_index, int16_t sequence_index, int16_t frame_index);

int glow_new(int glow_definition_index)
{
    if ( glow_definition_index == -1 )
        return -1;

    int glow_index = datum_new(glow_globals.glow_data);
    if ( glow_index != -1 )
    {
        glow_datum *glow = datum_get(glow_globals.glow_data, glow_index);
        glow_definition *definition = TAG_GET(glow_definition, glow_definition_index);
        int bitmap_group_index = definition->texture.index;
        bitmap_group *sprite_group = TAG_GET(bitmap_group, bitmap_group_index);

        if ( sprite_group->type == _bitmap_group_type_sprites )
        {
            bitmap_group_sequence *first_sequence = (bitmap_group_sequence *)sprite_group->sequences.address;
            bitmap_group_sprite *sprite = (bitmap_group_sprite *)first_sequence->sprites.address;
            bitmap_data *bitmap = bitmap_group_get_bitmap_from_sequence(bitmap_group_index, 0,
                                                                        sprite->bitmap_index);
            glow->definition_index = glow_definition_index;
            glow->number_of_particles = definition->number_of_particles;
            glow->bitmap_dimension = (int16_t)(int)((sprite->bounds.n[1] - sprite->bounds.n[0])
                                                  * (float)bitmap->width);
        }
    }
    return glow_index;
}
