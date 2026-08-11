/* biped_adjust_placement @0x837AC3C0 — for a biped whose pill is centered at its origin
 * (_biped_pill_centered_at_origin_bit) and that does not fly (_biped_flying_bit clear), nudge the
 * placement's position along its own "up" vector by the definition's collision_radius, so the pill's
 * base (feet) rather than its center lands at the placement point. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_placement_data.h"
#include "headers/biped_definition.h"
#include "headers/biped_definition_flags.h"
#include "headers/blam_data_globals.h"


void biped_adjust_placement(int object_index, object_placement_data *data)
{
    object_datum *object = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;

    biped_definition *definition = TAG_GET(biped_definition, object->definition_index);
    unsigned int flags = definition->biped.flags;

    if ( (flags & (1u << _biped_pill_centered_at_origin_bit)) != 0 && (flags & (1u << _biped_flying_bit)) == 0 )
    {
        float height_offset = definition->biped.collision_radius;
        data->position.n[0] += data->up.n[0] * height_offset;
        data->position.n[1] += data->up.n[1] * height_offset;
        data->position.n[2] += data->up.n[2] * height_offset;
    }
}
