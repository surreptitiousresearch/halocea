/* unit_impulse @0x836C88A0 — apply a velocity impulse to a unit. The supplied velocity is scaled by
 * mass / (object physics mass) and added to the object's translational velocity. No-op if the unit's
 * object definition has no physics tag. The origin argument is unused by the shipped code path. */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/_object_definition.h"
#include "headers/physics_definition.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

void unit_impulse(int unit_index, const real_point3d *origin, const real_vector3d *velocity, float mass)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    _object_definition *definition = TAG_GET(_object_definition, unit->definition_index);
    int physics_index = definition->physics.index;

    if (physics_index != -1)
    {
        physics_definition *physics = TAG_GET(physics_definition, physics_index);
        float scale = mass / physics->mass;

        unit->object.translational_velocity.n[0] += velocity->n[0] * scale;
        unit->object.translational_velocity.n[1] += velocity->n[1] * scale;
        unit->object.translational_velocity.n[2] += velocity->n[2] * scale;
    }
}
