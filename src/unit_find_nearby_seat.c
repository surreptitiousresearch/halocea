/* unit_find_nearby_seat @0x836D9... — for an AI unit, pick the best seat to board on a nearby vehicle. Each
 * of the vehicle definition's seats (stride 284 at definition+744) whose entrance/seat points are within one
 * world unit of the unit, whose seat label the unit is allowed to use, and which the unit may enter — either
 * directly (priority 2) or after evicting the current AI occupant (priority 1) — is scored by distance, with a
 * 1.5x penalty for leaving an already-chosen "special" seat for a non-special one. The highest-priority,
 * lowest-cost seat index is written to *parent_seat_index and its priority returned (0 = no seat found). The
 * search is skipped entirely if the vehicle is dead (object.damage_flags _object_dead_bit) or flagged
 * not-enterable (unit.flags _unit_not_enterable_by_player_bit). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/unit_seat_flags.h"
#include "headers/object_damage_flags.h"
#include "headers/unit_flags.h"
#include "headers/blam_data_globals.h"
extern float __fsqrts(float);


extern uint8_t unit_get_seat_entrance_point(int unit_index, int parent_unit_index, int16_t seat_index, real_point3d *entrance_point, real_point3d *seat_point, real_point3d *hint_point);
extern uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label, const char *weapon_label, uint8_t change_flag);
extern uint8_t unit_can_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index, int *occupant_unit_index_reference);
extern uint8_t ai_try_vehicle_eviction(int actor_index, int requesting_unit_index, uint8_t actually_evict);

int16_t unit_find_nearby_seat(int unit_index, int parent_unit_index, int16_t *parent_seat_index)
{
    unit_datum *vehicle_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_unit_index)->datum);
    unit_datum *unit_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit_definition *vehicle_definition = TAG_GET(unit_definition, vehicle_object->definition_index);

    int best_priority = 0;
    __int16 best_seat = -1;

    if ((vehicle_object->object.damage_flags & (1u << _object_dead_bit)) != 0 || (vehicle_object->unit.flags & (1u << _unit_not_enterable_by_player_bit)) != 0)
    {
        *parent_seat_index = best_seat;
        return best_priority;
    }

    char best_seat_is_special = 0;
    float best_cost = 3.4028235e38f;

    int seat_count = vehicle_definition->unit.seats.count;
    for (__int16 seat_index = 0; seat_index < seat_count; seat_index = (__int16)(seat_index + 1))
    {
        unit_seat *seat = &((unit_seat *)vehicle_definition->unit.seats.address)[seat_index];

        real_point3d entrance_point;
        real_point3d seat_point;
        if (!unit_get_seat_entrance_point(unit_index, parent_unit_index, seat_index, &entrance_point,
                                          &seat_point, nullptr))
            continue;

        const real_point3d *center = &unit_object->object.bounding_sphere_center;
        float entrance_distance = __fsqrts((center->n[0] - entrance_point.n[0]) * (center->n[0] - entrance_point.n[0])
                                         + (center->n[2] - entrance_point.n[2]) * (center->n[2] - entrance_point.n[2])
                                         + (center->n[1] - entrance_point.n[1]) * (center->n[1] - entrance_point.n[1]));
        float seat_distance = __fsqrts((center->n[0] - seat_point.n[0]) * (center->n[0] - seat_point.n[0])
                                     + (center->n[2] - seat_point.n[2]) * (center->n[2] - seat_point.n[2])
                                     + (center->n[1] - seat_point.n[1]) * (center->n[1] - seat_point.n[1]));
        float distance = (entrance_distance <= seat_distance) ? entrance_distance : seat_distance;
        if (distance >= 1.0f)
            continue;

        int seat_flags = seat->flags;
        int priority = 0;
        if ((((seat_flags >> _unit_seat_not_valid_without_driver_bit) & 1) == 0 || vehicle_object->unit.driver_object_index != -1)
            && seat->label[0]
            && unit_set_or_test_seat_and_weapon_label(unit_index, seat->label, nullptr, 0))
        {
            int occupant_unit = -1;
            if (unit_can_enter_seat(unit_index, parent_unit_index, seat_index, &occupant_unit))
            {
                priority = 2;
            }
            else if (occupant_unit != -1)
            {
                unit_datum *occupant_object = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, occupant_unit)->datum);
                int occupant_actor = occupant_object->unit.actor_index;
                if (occupant_actor != -1 && ai_try_vehicle_eviction(occupant_actor, unit_index, 0))
                    priority = 1;
            }
        }

        if ((__int16)priority > 0)
        {
            float cost_multiplier = 1.0f;
            if (best_seat_is_special && ((seat_flags >> _unit_seat_is_driver_bit) & 1) == 0)
                cost_multiplier = 1.5f;

            char take_seat = 0;
            if (best_seat == -1 || (__int16)priority > (__int16)best_priority || cost_multiplier * distance < best_cost)
                take_seat = 1;
            if (take_seat)
            {
                best_priority = priority;
                best_cost = distance;
                best_seat = seat_index;
                best_seat_is_special = (seat_flags >> _unit_seat_is_driver_bit) & 1;
            }
        }
    }

    *parent_seat_index = best_seat;
    return best_priority;
}
