/* actor_combat_find_grenade_target @0x837B82A0 — pick a grenade throw target from the actor's current
 * attack-of-opportunity prop: requires the prop to be a live (not really-dead) enemy, in a firing-related
 * prop state (2, 3, or 4), and at a distance within the actor variant's grenade throw range ([min,max)
 * from the tag definition at word offsets 404/408). On success, targets the prop's body position (nudged
 * up 0.2 units), reports the prop and an object to ignore for the throw (the prop's vehicle, if any), and
 * — if the actor is in a scripted "playfighting" order — additionally looks for a safer nearby target
 * to throw at instead. Returns 1 if a target was found, 0 otherwise.
 *
 * DEVIATION: byte offset 458 in actor_datum, per the sibling ai_release_inactive_encounters.c/
 * actor_action_handle_grenade_throwing.c convention of raw-actor-offset access, resolves cleanly to
 * actor->external_orders.playfighting (456 + 2). The tag definition's min/max grenade range fields
 * (offsets 404/408) are variant_tag->grenade_combat.grenade_range_lower_bound/upper_bound. */

#include <stdint.h>
#include "headers/actor_variant_definition.h"
#include "headers/actor_datum.h"
#include "headers/data_array.h"
#include "headers/prop_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/prop_perception_state.h"
#include "headers/blam_data_globals.h"


extern void actor_combat_find_nearby_target(real_point3d *target_point, float miss_distance);

uint8_t actor_combat_find_grenade_target(int actor_index, real_point3d *grenade_target, int *grenade_target_prop_index, int *grenade_ignore_object_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int found = 0;

    /* actor variant tag definition — grenade_combat.grenade_range_lower/upper_bound (abs +404/+408). */
    const actor_variant_definition *variant_tag =
        TAG_GET(const actor_variant_definition, actor->meta.variant_definition_index);

    /* recovered: raw dword offset 624 -> actor->target.target_prop_index */
    if ( actor->target.target_prop_index != -1 )
    {
        prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, actor->target.target_prop_index);

        if ( prop->enemy && !prop->really_dead )
        {
            if ( (prop->state >= _prop_state_becoming_unacknowledged && prop->state <= _prop_state_acknowledged) || prop->state == _prop_state_uninspected_orphan )
            {
                if ( prop->distance > variant_tag->grenade_combat.grenade_range_lower_bound
                  && prop->distance < variant_tag->grenade_combat.grenade_range_upper_bound )
                {
                    found = 1;
                    *grenade_target = prop->body_position;
                    grenade_target->n[2] += 0.2f;
                    *grenade_target_prop_index = actor->target.target_prop_index;
                    *grenade_ignore_object_index = prop->vehicle_index;
                }
            }
        }
    }

    if ( found && actor->external_orders.playfighting )
        actor_combat_find_nearby_target(grenade_target, 1.5f);

    return found;
}
