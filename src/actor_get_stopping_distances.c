/* actor_get_stopping_distances @0x837C7188 — estimate how far the actor's controlled unit (on foot) or vehicle
 * will travel before it can halt, both at its current speed and at its reference (max) speed, so movement
 * planning can begin braking in time. Current speed is the dot of the object's velocity and heading vectors
 * (object+104 . object+116). Deceleration rates come from the unit/vehicle tag when it provides them (flag bit
 * 4), otherwise sensible defaults; a boosting vehicle scales them by the tag's boost multiplier. Either output
 * pointer may be null.
 *
 * DEVIATION: register-pun doubles are plain float math. 2026-07-14 fully typed: the on-foot branch's
 * "deceleration data" is the biped tag's FLYING block (flags bit 4 = flies; flying_velocity/acceleration/
 * deceleration x1/30, crouch modifier), and the "boost multiplier" was flying_crouch_velocity_modifier. */

#include "headers/actor_datum.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/biped_definition.h"
#include "headers/vehicle_definition.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/biped_definition_flags.h"
#include "headers/object_type.h"
#include "headers/actor_vehicle_driver_type.h"
#include "headers/blam_data_globals.h"
#include "headers/game_time_constants.h"


extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);

void actor_get_stopping_distances(int actor_index, float *current_stopping_distance_reference,
        float *maximum_stopping_distance_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    float current_speed = 0.0f;
    float reference_speed = 0.083333336f;         /* default */
    float deceleration = 0.016666668f;            /* default */
    float stopping_deceleration = 0.026666667f;   /* default */

    if ( actor->input.vehicle_index == -1 )   /* on foot */
    {
        int unit_index = actor->meta.unit_index;
        if ( unit_index != -1 )
        {
            unit_datum *unit = object_try_and_get_and_verify_type(unit_index, object_mask_biped);
            if ( unit )
            {
                biped_definition *definition = TAG_GET(biped_definition, unit->definition_index);
                current_speed = unit->object.forward.n[0] * unit->object.translational_velocity.n[0]
                              + (unit->object.translational_velocity.n[2] * unit->object.forward.n[2]
                                 + unit->object.translational_velocity.n[1] * unit->object.forward.n[1]);
                if ( (definition->biped.flags & (1u << _biped_flying_bit)) != 0 )   /* flies */
                {
                    reference_speed = definition->biped.flying_velocity * SECONDS_PER_TICK;
                    deceleration = definition->biped.flying_acceleration * SECONDS_PER_TICK;
                    stopping_deceleration = definition->biped.flying_deceleration * SECONDS_PER_TICK;
                    if ( actor->control.crouching && definition->biped.flying_crouch_velocity_modifier > 0.0f )
                    {
                        reference_speed = definition->biped.flying_crouch_velocity_modifier * (definition->biped.flying_velocity * SECONDS_PER_TICK);
                        deceleration = definition->biped.flying_crouch_velocity_modifier * (definition->biped.flying_acceleration * SECONDS_PER_TICK);
                        stopping_deceleration = definition->biped.flying_crouch_velocity_modifier * (definition->biped.flying_deceleration * SECONDS_PER_TICK);
                    }
                }
            }
        }
    }
    else   /* in a vehicle seat */
    {
        unsigned int seat_state = actor->input.vehicle_driver_type;
        if ( seat_state >= _actor_vehicle_driver_hovering_ground
             && seat_state <= _actor_vehicle_driver_nondirectional_ground )
        {
            int vehicle_index = actor->input.vehicle_index;
            vehicle_datum *vehicle = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum);
            vehicle_definition *definition = TAG_GET(vehicle_definition, vehicle->definition_index);
            deceleration = definition->vehicle.speed.acceleration;  /* header FIX: acc/dec offsets were once swapped; this is rel 0x10 */
            reference_speed = definition->vehicle.speed.maximum_forward_speed;
            stopping_deceleration = deceleration;
            current_speed = vehicle->object.translational_velocity.n[0] * vehicle->object.forward.n[0]
                + (vehicle->object.translational_velocity.n[2] * vehicle->object.forward.n[2]
                 + vehicle->object.translational_velocity.n[1] * vehicle->object.forward.n[1]);
        }
    }

    if ( current_stopping_distance_reference )
        *current_stopping_distance_reference = current_speed * current_speed / (stopping_deceleration * 2.0f);
    if ( maximum_stopping_distance_reference )
    {
        if ( current_speed > reference_speed )
            reference_speed = current_speed;
        *maximum_stopping_distance_reference =
            -(current_speed * current_speed - reference_speed * reference_speed) / (deceleration * 2.0f)
            + reference_speed * reference_speed / (stopping_deceleration * 2.0f);
    }
}
