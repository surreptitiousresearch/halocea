/* action_vehicle_evaluate_seat @0x83821698 — checks whether `actor_index` can currently claim `seat_index` of
 * `vehicle_index`: the seat must be unfilled, allow noncombatants if the actor's own definition requires it, and
 * have a valid entrance/seat/hint point triple (unit_get_seat_entrance_point). If so, scans every other actor
 * already mid-"go to vehicle" (action class 9, see ai_scripting_going_to_vehicle.c) for the same vehicle+seat, and
 * yields (returns 0) if one of them is already closer to actually reaching it than we are. Otherwise computes and
 * returns the entry point/facing/hint point, a distance-based seat weight (closer is better; a further bonus is
 * added when the actor variant's gunner-affinity flag disagrees with whether the seat itself is a gunner seat),
 * and three cheap proximity/facing booleans the caller can use without redoing the distance math.
 *
 * DEVIATION (resolved, not a real corruption): disasm_range(0x838216A8,0x83821720) traces the two
 * TAG_INSTANCE(...)-derived reads (actor's definition/variant-definition pointers) precisely. `lwz r29, 0x14(r6)`
 * / `lwz r22, 0x14(r5)` look at first glance like they might apply the TAG_INSTANCE macro's own +0x14
 * `.base_address` offset a second time, but r6/r5 here are just `&global_tag_instances[index]` (struct base,
 * offset 0) — the `+0x14` in each `lwz` IS the macro's `.base_address` offset, applied exactly once. The
 * decompiler's plain `*(TAG_INSTANCE(...))` reading was correct all along; an earlier session's doubt on this
 * point was a mistraced register, not a real signature/body corruption.
 *
 * The three out-of-band out-params in the raw decompile (a28/a30/a32, fabricated as disconnected locals) are the
 * DB's own 11-param prototype's trailing `within_range_reference`/`correct_facing_reference`/
 * `could_potentially_fake_reference` — Hex-Rays fails to bind these late stack-passed (arg_54/5C/64) out-params to
 * the locals that actually compute them (named `within_range`/`correct_facing`/`could_potentially_fake` below).
 *
 * FAITHFUL: `object_get_origin(vehicle_index, ...)` is called but its output is never read afterward anywhere in
 * the function — reproduced as-is (the original binary does this too) rather than dropped, since the call may
 * have side effects beyond filling the point. Likewise both `unit_seat_is_driver` calls below have their return
 * values discarded in the original — kept for fidelity. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/actor_definition.h"
#include "headers/actor_variant_definition.h"
#include "headers/actor_variant_definition_flags.h"
#include "headers/actor_definition_flags2_flags.h"
#include "headers/global_tag_instances.h"
#include "headers/prop_iterator.h"
#include "headers/prop_datum.h"
#include "headers/vehicle_state_data.h"
#include "headers/real_point3d.h"
#include "headers/real_vector2d.h"
#include "headers/real_vector3d.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"

extern float __fsqrts(float x);
extern float normalize2d(real_vector2d *v);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern uint8_t unit_seat_filled(int parent_object_index, int16_t seat_index);
extern uint8_t unit_seat_allow_noncombatants(int vehicle_index, int16_t seat_index);
extern uint8_t unit_seat_is_gunner(int vehicle_index, int16_t seat_index);
extern uint8_t unit_seat_is_driver(int vehicle_index, int16_t seat_index);
extern uint8_t unit_get_seat_entrance_point(int unit_index, int parent_unit_index, int16_t seat_index, real_point3d *entrance_point, real_point3d *seat_point, real_point3d *hint_point);
extern void prop_iterator_new(prop_iterator *iterator, uint16_t actor_index);
extern prop_datum *prop_iterator_next(prop_iterator *iterator);
extern void *datum_try_and_get(const data_array *data, int index);

uint8_t action_vehicle_evaluate_seat(int actor_index, int vehicle_index, int16_t seat_index, uint8_t allow_any_seat, real_point3d *entry_point, real_vector3d *entry_facing, real_point3d *hint_point, float *seat_weight_reference, uint8_t *within_range_reference, uint8_t *correct_facing_reference, uint8_t *could_potentially_fake_reference)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    unsigned __int8 result = 0;

    actor_definition *definition = TAG_GET(actor_definition, actor->meta.definition_index);
    actor_variant_definition *variant =
        TAG_GET(actor_variant_definition, actor->meta.variant_definition_index);

    real_point3d entrance_point, seat_point, seat_hint_point;
    if ( unit_seat_filled(vehicle_index, seat_index)
      || ((definition->flags2 & (1u << _actor_definition_flags2_disallow_vehicle_combat_bit)) != 0 && !unit_seat_allow_noncombatants(vehicle_index, seat_index))
      || !unit_get_seat_entrance_point(actor->meta.unit_index, vehicle_index, seat_index,
                &entrance_point, &seat_point, &seat_hint_point) )
    {
        return result;
    }
    result = 1;

    real_point3d vehicle_origin; /* filled but never read afterward — matches the original */
    object_get_origin(vehicle_index, &vehicle_origin);

    real_vector2d entry_direction;
    entry_direction.n[0] = seat_point.n[0] - entrance_point.n[0];
    entry_direction.n[1] = seat_point.n[1] - entrance_point.n[1];

    real_vector2d computed_facing;
    float computed_facing_z;
    if ( normalize2d(&entry_direction) == 0.0f )
    {
        computed_facing = *(real_vector2d *)actor->input.facing_vector.n;
        computed_facing_z = actor->input.facing_vector.n[2];
    }
    else
    {
        computed_facing_z = 0.0f;
        computed_facing = entry_direction;
    }

    float entrance_dx = entrance_point.n[0] - actor->input.position.body_position.n[0];
    float seat_dx     = seat_point.n[0]     - actor->input.position.body_position.n[0];
    float entrance_dy = entrance_point.n[1] - actor->input.position.body_position.n[1];
    float seat_dy     = seat_point.n[1]     - actor->input.position.body_position.n[1];

    float nearest_distance_sq;
    if ( __fsqrts(entrance_dy * entrance_dy + entrance_dx * entrance_dx)
      <= __fsqrts(seat_dy * seat_dy + seat_dx * seat_dx) )
        nearest_distance_sq = entrance_dy * entrance_dy + entrance_dx * entrance_dx;
    else
        nearest_distance_sq = seat_dy * seat_dy + seat_dx * seat_dx;
    float distance_to_seat = __fsqrts(nearest_distance_sq);

    prop_iterator prop_cursor;
    prop_iterator_new(&prop_cursor, actor_index);
    for ( prop_datum *prop = prop_iterator_next(&prop_cursor); prop; prop = prop_iterator_next(&prop_cursor) )
    {
        if ( !prop->enemy )
        {
            int other_actor_index = prop->actor_index;
            if ( other_actor_index != -1 )
            {
                actor_datum *other_actor = datum_try_and_get(actor_data, other_actor_index);
                if ( other_actor->state.action == actor_action_vehicle )
                {
                    vehicle_state_data *other_vehicle_action =
                        &other_actor->state.action_data.___u0.vehicle;
                    if ( other_vehicle_action->vehicle_index == vehicle_index )
                    {
                        if ( other_vehicle_action->seat_index == seat_index )
                        {
                            float other_dx = other_vehicle_action->destination_point.n[0]
                                - other_actor->input.position.body_position.n[0];
                            float other_dy = other_vehicle_action->destination_point.n[1]
                                - other_actor->input.position.body_position.n[1];
                            if ( other_dy * other_dy + other_dx * other_dx
                              < distance_to_seat * distance_to_seat )
                            {
                                result = 0;
                                break;
                            }
                        }
                        else
                        {
                            unit_seat_is_driver(vehicle_index, other_vehicle_action->seat_index);
                        }
                    }
                }
            }
        }
    }
    if ( !result )
        return result;

    if ( !allow_any_seat )
        unit_seat_is_driver(vehicle_index, seat_index);

    real_vector2d to_seat_direction;
    to_seat_direction.n[0] = seat_point.n[0] - actor->input.position.body_position.n[0];
    to_seat_direction.n[1] = seat_point.n[1] - actor->input.position.body_position.n[1];
    normalize2d(&to_seat_direction);
    float facing_dot = actor->input.facing_vector.n[1] * to_seat_direction.n[1]
                      + actor->input.facing_vector.n[0] * to_seat_direction.n[0];

    unsigned __int8 within_range = distance_to_seat < 0.7f;
    unsigned __int8 correct_facing = facing_dot > 0.6f;
    unsigned __int8 could_potentially_fake = distance_to_seat < 1.1f && facing_dot > 0.0f;

    float seat_weight = 10.0f / (distance_to_seat + 1.0f);
    if ( (variant->flags & (1u << _actor_variant_definition_prefer_passenger_seat_bit)) != 0 )  /* gunner-affinity variant flag */
    {
        if ( !unit_seat_is_gunner(vehicle_index, seat_index) )
            seat_weight += 3.5f;
    }
    else if ( unit_seat_is_gunner(vehicle_index, seat_index) )
    {
        seat_weight += 3.5f;
    }

    if ( entry_point )
        *entry_point = entrance_point;
    if ( entry_facing )
    {
        *(real_vector2d *)entry_facing->n = computed_facing;
        entry_facing->n[2] = computed_facing_z;
    }
    if ( hint_point )
        *hint_point = seat_hint_point;
    if ( seat_weight_reference )
        *seat_weight_reference = seat_weight;
    if ( within_range_reference )
        *within_range_reference = within_range;
    if ( correct_facing_reference )
        *correct_facing_reference = correct_facing;
    if ( could_potentially_fake_reference )
        *could_potentially_fake_reference = could_potentially_fake;
    return 1;
}
