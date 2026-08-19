/* player_examine_nearby_vehicle @0x836ABED8 — when a player is looking at a nearby vehicle, decide which
 * "enter/flip" action prompt (if any) to offer. If the player is looking at the vehicle's underside (pitch
 * past the control deadzone) and the vehicle isn't already flipping and is unoccupied, offer the flip
 * action (11). Otherwise, provided neither the player's unit nor the vehicle is moving and the player
 * isn't overcharged, look for a nearby seat and offer evict-from-vehicle (nearby_seat==1) or enter-vehicle (nearby_seat==2).
 *
 * DEVIATION: the decompiler invents a phantom `long double a3` (float-slot ABI artifact); the real
 * signature takes no third argument and the slot is reused as the cos() angle local. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/player_datum.h"
#include "headers/game_globals_tag.h"
#include "headers/game_globals_player_control.h"
#include "headers/object_damage_flags.h"
#include "headers/vehicle_flags.h"
#include "headers/player_action_result.h"
#include "headers/blam_data_globals.h"


extern double cos(double x);
extern uint8_t unit_overcharged(int unit_index);
extern int16_t unit_find_nearby_seat(int unit_index, int parent_unit_index, int16_t *parent_seat_index);
extern void player_set_action_result(int player_index, int16_t action_result, int action_object_index, int16_t action_seat_index);

void player_examine_nearby_vehicle(int player_index, int vehicle_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    vehicle_datum *vehicle = (vehicle_datum *)
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_index)->datum;

    if ((vehicle->object.damage_flags & (1u << _object_dead_bit)) != 0)
        return;

    float look_angle = (1.5707964f - ((game_globals_player_control *)global_game_globals->player_control.address)->minimum_vehicle_flipping_angle);
    float cos_threshold = cos(look_angle);

    if (vehicle->object.up.n[2] <= (double)cos_threshold)
    {
        if ((vehicle->vehicle.flags & (1u << _vehicle_upending_bit)) == 0 && vehicle->unit.driver_object_index == -1)
            player_set_action_result(player_index, _player_action_result_flip_vehicle, vehicle_index, -1);
    }
    else if (!unit_overcharged(player->unit_index))
    {
        int unit_index = player->unit_index;
        object_datum *unit = DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                unit_index)->datum;
        real_vector3d *uv = &unit->object.translational_velocity;
        if (((uv->n[2] * uv->n[2])
                  + ((uv->n[0] * uv->n[0]) + (uv->n[1] * uv->n[1]))) < 0.0099999998f)
        {
            real_vector3d *vv = &vehicle->object.angular_velocity;
            if (((vv->n[2] * vv->n[2])
                      + ((vv->n[0] * vv->n[0])
                              + (vv->n[1] * vv->n[1]))) < 0.0099999998f)
            {
                int16_t parent_seat_index = -1;
                int16_t seat_result = unit_find_nearby_seat(unit_index, vehicle_index, &parent_seat_index);
                if (seat_result == 1)
                    player_set_action_result(player_index, _player_action_result_evict_from_vehicle, vehicle_index, parent_seat_index);
                else if (seat_result == 2)
                    player_set_action_result(player_index, _player_action_result_enter_vehicle, vehicle_index, parent_seat_index);
            }
        }
    }
}
