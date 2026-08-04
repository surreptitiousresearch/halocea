/* update_alien_fighter_physics @0x83761ED0 — dispatch to the "old" or "new" alien-fighter physics model based
 * on a threshold value read from the vehicle definition's referenced sub-definition (a physics tuning field
 * <= 0.0 selects the new model), then spawns the vehicle's ghost trail effect either way.
 *
 * DEVIATION: the decompiler expressed the tag lookups as a nested nnn**((...)) double-dereference chain.
 * Traced it to: object tag index (object+0, standard Blam convention) -> vehicle tag definition via
 * TAG_INSTANCE -> definition+140 = object.physics.index (DB-verified) -> the physics tag via TAG_INSTANCE
 * -> its first field, physics_definition.radius (float @0), used as the old/new-model threshold. Object
 * header lookup matches the established salted object-header idiom (see vehicle_reset.c). */

#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/vehicle_definition.h"
#include "headers/physics_definition.h"
#include "headers/powered_mass_point_datum.h"
#include "headers/mass_point_datum.h"
#include "headers/blam_data_globals.h"


extern void update_alien_fighter_physics_new(int vehicle_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void update_alien_fighter_physics_old(int vehicle_index, powered_mass_point_datum *powered_mass_points, mass_point_datum *mass_points);
extern void create_ghost_effect(int vehicle_index);

void update_alien_fighter_physics(int vehicle_index, powered_mass_point_datum *powered_mass_points,
    mass_point_datum *mass_points)
{
    int vehicle = ((int)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
    int object_tag_index = *(int *)vehicle;
    vehicle_definition *definition = TAG_GET(vehicle_definition, object_tag_index);
    physics_definition *physics = TAG_GET(physics_definition, definition->object.physics.index);

    if ( physics->radius <= 0.0f )
        update_alien_fighter_physics_new(vehicle_index, powered_mass_points, mass_points);
    else
        update_alien_fighter_physics_old(vehicle_index, powered_mass_points, mass_points);

    create_ghost_effect(vehicle_index);
}
