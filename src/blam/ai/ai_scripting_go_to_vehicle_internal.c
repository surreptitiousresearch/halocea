/* ai_scripting_go_to_vehicle_internal @0x83770D48 — order the actors addressed by an AI index to enter a
 * vehicle. Gathers every addressed actor as a candidate (recording its squared distance to the vehicle and
 * whether it is already heading to a vehicle), sorts them (closest, not-already-going first), then dispatches
 * each in order. When override_previous_vehicles is false, dispatch stops at the first candidate already
 * heading to a vehicle. */

#include <stdint.h>
#include "headers/actor_datum.h"
#include "headers/ai_index_actor_iterator.h"
#include "headers/vehicle_scripting_candidate.h"
#include "headers/real_point3d.h"
#include "headers/actor_action.h"
#include "headers/object_type.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern real_point3d *object_get_origin(int object_index, real_point3d *origin);
extern int16_t vehicle_scripting_find_available_seats(int unit_index, const char *seat_substring_name, int16_t seat_desire_type, int16_t *seat_indices, int16_t seat_max_indices);
extern void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator);
extern actor_datum *ai_index_actor_iterator_next(ai_index_actor_iterator *iterator);
extern void qsort(void *base, unsigned int count, unsigned int size,
                  int (*compare)(const void *, const void *));
extern int ai_scripting_vehicle_candidate_qsort(const void *a, const void *b);
extern uint8_t actor_action_try_to_enter_vehicle(int actor_index, int vehicle_index, const char *seat_substring_name, int16_t seat_desire_type, int16_t precomputed_seat_count, int16_t *precomputed_seat_indices);

void ai_scripting_go_to_vehicle_internal(int ai_index, int unit_index, const char *seat_substring_name, uint8_t override_previous_vehicles)
{
    void *vehicle = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
    if ( ai_index == -1 || !vehicle )
        return;

    int16_t candidate_count = 0;
    real_point3d vehicle_origin;
    object_get_origin(unit_index, &vehicle_origin);

    int16_t seat_indices[16];
    int16_t available_seats = vehicle_scripting_find_available_seats(unit_index, seat_substring_name, -1, seat_indices, 16);
    if ( available_seats <= 0 )
        return;

    vehicle_scripting_candidate candidates[64];
    ai_index_actor_iterator iterator;
    ai_index_actor_iterator_new(ai_index, &iterator);
    for ( actor_datum *actor = ai_index_actor_iterator_next(&iterator);
          actor;
          actor = ai_index_actor_iterator_next(&iterator) )
    {
        if ( candidate_count < 0x40u )
        {
            vehicle_scripting_candidate *candidate = &candidates[candidate_count++];
            candidate->actor_index = iterator.iterator.index;
            float dx = vehicle_origin.n[0] - actor->input.position.body_position.n[0];
            float dy = vehicle_origin.n[1] - actor->input.position.body_position.n[1];
            float dz = vehicle_origin.n[2] - actor->input.position.body_position.n[2];
            candidate->distance_squared = dz * dz + (dx * dx + dy * dy);
            candidate->already_going_to_vehicle = actor->state.action == actor_action_vehicle;   /* action class 9 = go to vehicle */
        }
    }

    int total = candidate_count;
    qsort(candidates, candidate_count, sizeof(vehicle_scripting_candidate), ai_scripting_vehicle_candidate_qsort);
    for ( int i = 0; i < total; i = (int16_t)(i + 1) )
    {
        if ( candidates[i].already_going_to_vehicle && !override_previous_vehicles )
            break;
        actor_action_try_to_enter_vehicle(candidates[i].actor_index, unit_index, nullptr, -1, available_seats, seat_indices);
    }
}
