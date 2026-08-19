/* glow_render @0x8380C1B8 — render hook for one glow attachment: open a sprite batch sized to the glow's
 * live particle count over the glow definition's bitmap (glow_definition.texture.index, +336) with the additive effect
 * shader, then emit one sprite per particle (position, the parent marker's axis from the marker matrix —
 * the compiled address is glow+108*marker+0x44, i.e. 0x3C into the 108-byte object_marker — present size,
 * color and fade; no rotation, flags 0).
 *
 * DEVIATION (bug class 7): the decompiler scrambled the build_sprite call (a word cast to the color
 * pointer, the color address passed as flags); disasm gives the mapping used here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/glow_globals.h"
#include "headers/glow_datum.h"
#include "headers/glow_definition.h"
#include "headers/glow_particle.h"
#include "headers/build_sprite_data.h"
#include "headers/shader_effect.h"
#include "headers/blam_data_globals.h"


#include "headers/shader_effect.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/real_argb_color.h"
extern void *datum_get(data_array *data, int index);
extern void build_sprites_begin(build_sprite_data *data, int16_t maximum_sprite_count, int bitmap_group_index, const shader_effect *shader, unsigned int flags);
extern void build_sprite(build_sprite_data *data, int16_t mode, int16_t sequence_index, int16_t sprite_index, const real_point3d *untransformed_origin, const real_vector3d *untransformed_direction, float rotation, float scale, const real_argb_color *color, float fade, unsigned int flags);
extern void build_sprites_end(build_sprite_data *data);

void glow_render(int object_index, int widget_index)
{
    glow_datum *glow = datum_get(glow_globals.glow_data, widget_index);
    glow_definition *definition = TAG_GET(glow_definition, glow->definition_index);

    build_sprite_data sprite_data;
    build_sprites_begin(&sprite_data, glow->number_of_particles,
                        definition->texture.index,
                        &global_shader_effect_additive, 0);

    for ( glow_particle *particle = glow->head_particle; particle; particle = particle->next )
    {
        /* recovered: (char *)glow + 108*marker + 0x44 -> markers[marker].matrix.forward */
        build_sprite(&sprite_data, 0, 0, 0, &particle->position,
                     &glow->markers[particle->parent_marker_index].matrix.forward,
                     0.0f, particle->present_size, &particle->color, particle->fade, 0);
    }

    build_sprites_end(&sprite_data);
}
