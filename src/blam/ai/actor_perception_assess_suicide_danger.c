/* actor_perception_assess_suicide_danger @0x837D6378 — record an incoming suicide (kamikaze) threat from a
 * unit within suicide_radius + 10. If the actor has no current suicide threat, or has a weaker/farther one
 * that is a different unit, this threat replaces it: the threat block (actor +0x280) is reset and populated
 * with the unit, its origin and velocity, a 6-tick fuse, the enemy flag and the perceived flag. Returns 1 if a
 * threat was (re)latched.
 *
 * Deviation: the database prototype's 5th/6th params (enemy, currently_perceived) are the real args, but the
 * float-slot-skip ABI makes Hex-Rays read them through two invented trailing slots. Those are what the body
 * consumes: enemy -> danger_zone.hostility (stored inverted), currently_perceived -> danger_zone.currently_perceived. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/real_point3d.h"
#include <string.h>
#include "headers/actor_danger_zone_type.h"
#include "headers/blam_data_globals.h"

extern real_point3d *object_get_origin(int object_index, real_point3d *origin);

int actor_perception_assess_suicide_danger(int actor_index, int unit_index, float suicide_radius,
                                           float current_distance, uint8_t enemy,
                                           uint8_t currently_perceived)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    if ( current_distance < (suicide_radius + 10.0f) )
    {
        int threat_count = actor->danger_zone.danger_type;
        if ( threat_count < actor_danger_zone_suicide
          || (threat_count == actor_danger_zone_suicide && actor->danger_zone.object_index != unit_index
              && current_distance < actor->danger_zone.current_distance_from_actor) )
        {
            unit_datum *object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
            memset(&actor->danger_zone, 0, 0x6Cu);
            actor->danger_zone.danger_radius = suicide_radius;
            actor->danger_zone.object_index = unit_index;
            actor->danger_zone.danger_type = actor_danger_zone_suicide;
            object_get_origin(unit_index, &actor->danger_zone.initial_position);
            actor->danger_zone.initial_velocity.n[0] = object->object.translational_velocity.n[0];
            actor->danger_zone.initial_velocity.n[1] = object->object.translational_velocity.n[1];
            actor->danger_zone.initial_velocity.n[2] = object->object.translational_velocity.n[2];
            actor->danger_zone.acknowledgement_timer = 6;
            actor->danger_zone.currently_perceived = currently_perceived;
            actor->danger_zone.hostility = enemy == 0;
            return 1;
        }
    }
    return 0;
}
