/* actor_get_weapon_vector @0x837B7A50 — get the direction an actor's weapon points. If the actor is using
 * a special weapon-aim object (actor +353 set) whose definition has the "use object forward" flag
 * (tag +752, bit 0x100), use that object's forward vector (object +116). Otherwise fall back to the unit's
 * aiming vector, clipped to the unit's aiming bounds.
 * Raw actor-datum offsets (stride 1828) reproduced from the decompiler. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/vehicle_definition_flags.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/object_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/global_tag_instances.h"
#include "headers/real_vector3d.h"
#include "headers/blam_data_globals.h"

extern void unit_get_aiming_vector(int unit_index, real_vector3d *aiming_vector);
extern int unit_clip_to_aiming_bounds(int unit_index, real_vector3d *vector, uint8_t use_aiming_screen);

void actor_get_weapon_vector(int actor_index, real_vector3d *weapon_vector)
{
    char have_vector = 0;
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int unit_index = actor->meta.unit_index;
    if ( actor->input.vehicle_gunner )
    {
        unit_index = actor->input.vehicle_index;
        object_datum *object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
        vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, object->definition_index);
        if ( (vehicle_def->vehicle.flags & (1u << _vehicle_ai_weapon_cannot_rotate_bit)) != 0 )  /* def+752: "use object forward" */
        {
            have_vector = 1;
            *weapon_vector = object->object.forward;   /* object+116 = _object_datum.forward */
        }
    }
    if ( !have_vector )
    {
        unit_get_aiming_vector(unit_index, weapon_vector);
        unit_clip_to_aiming_bounds(unit_index, weapon_vector, 1u);
    }
}
