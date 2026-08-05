/* actor_action_try_to_enter_vehicle @0x837F3830 — try to switch an actor into the "enter vehicle" action,
 * trying each available seat in turn until one is reachable (the actor has an entry animation for it and
 * the vehicle action set up successfully). If a precomputed seat list is not supplied, it is computed here.
 * Returns 1 if the actor began entering a seat (which is then marked consumed in the seat list), 0 if no
 * seat worked. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/action_state_data.h"
#include "headers/vehicle_state_data.h"
#include "headers/actor_action.h"
#include "headers/blam_data_globals.h"

#include "headers/vehicle_state_data.h"
extern int16_t vehicle_scripting_find_available_seats(int unit_index, const char *seat_substring_name,
                                                      int16_t seat_desire_type, int16_t *seat_indices,
                                                      int16_t seat_max_indices);
extern uint8_t unit_has_animation_to_enter_seat(int unit_index, int vehicle_index, int16_t seat_index);
extern uint8_t action_vehicle_setup_specific(int actor_index, int vehicle_index, int16_t seat_index, vehicle_state_data *state_data);
extern void actor_action_change(int actor_index, int new_action_type, action_state_data *new_action_data);

uint8_t actor_action_try_to_enter_vehicle(int actor_index, int vehicle_index, const char *seat_substring_name, int16_t seat_desire_type, int16_t precomputed_seat_count, int16_t *precomputed_seat_indices)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    int16_t local_seat_indices[16];
    int16_t *seat_indices;
    if ( precomputed_seat_indices )
    {
        seat_indices = precomputed_seat_indices;
    }
    else
    {
        seat_indices = local_seat_indices;
        precomputed_seat_count = vehicle_scripting_find_available_seats(vehicle_index, seat_substring_name,
                                                                        seat_desire_type, local_seat_indices, 16);
    }

    int seat_count = precomputed_seat_count;
    if ( precomputed_seat_count <= 0 )
        return 0;

    action_state_data vehicle_action;
    int i = 0;
    while ( 1 )
    {
        int16_t seat_index = (uint16_t)seat_indices[i];
        if ( (uint16_t)seat_index != 0xFFFF
          && unit_has_animation_to_enter_seat(actor->meta.unit_index, vehicle_index, seat_indices[i])
          && action_vehicle_setup_specific(actor_index, vehicle_index, seat_index, &vehicle_action.___u0.vehicle) )
        {
            break;
        }
        i = (int16_t)(i + 1);
        if ( i >= seat_count )
            return 0;
    }

    actor_action_change(actor_index, actor_action_vehicle, &vehicle_action);
    seat_indices[i] = -1;   /* consume the chosen seat */
    return 1;
}
