/* biped_get_physics_pill @0x837AC9F0 — compute the collision pill for a biped: its base point (object origin,
 * optionally lifted by the model's collision radius), the pill half-height, and the pill width (radius). When the
 * biped is spherical (_biped_spherical_bit) or has no player and isn't a movie star
 * (_object_movie_star_bit), the pill collapses to zero height. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_definition.h"
#include "headers/real_point3d.h"
#include "headers/global_tag_instances.h"
#include "headers/object_flags.h"
#include "headers/biped_definition_flags.h"
#include "headers/blam_data_globals.h"
/* DEVIATION: the decompiler modeled global_tag_instances as {base_address@0}; the DB-verified entry
 * (cache_file_tag_instance, 32 bytes) has base_address@0x14 — disasm reads `lwz r31, 0x14(r11)`. The
 * field name matches, so reusing the canonical header corrects the tag-instance offset. */


extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

void biped_get_physics_pill(int biped_index, real_point3d *base, float *height, float *width)
{
    biped_datum *object = (biped_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, biped_index)->datum;
    biped_definition *definition = TAG_GET(biped_definition, object->definition_index);

    object_get_origin(biped_index, base);
    if ( (definition->biped.flags & (1u << _biped_pill_centered_at_origin_bit)) == 0 )
        base->n[2] = definition->biped.collision_radius + base->n[2];

    if ( (definition->biped.flags & (1u << _biped_spherical_bit)) != 0
      || object->unit.player_index == -1 && (object->object.flags & (1u << _object_movie_star_bit)) == 0 )
    {
        *height = 0.0;
        *width = definition->biped.collision_radius;
    }
    else
    {
        *height = -((definition->biped.collision_radius * (float)2.0)
                          - (((definition->biped.collision_height_crouching
                                                  - definition->biped.collision_height_standing)
                                          * object->biped.crouch)
                                  + definition->biped.collision_height_standing));
        *width = definition->biped.collision_radius;
    }
}
