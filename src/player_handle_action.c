/* player_handle_action @ 0x836A9DA8 — carry out a player's pending "action" (the contextual X-button
 * action) based on the action type stored in the player datum (word +40):
 *   5  — pick up equipment/powerup (drop current, add to inventory, HUD message);
 *   6,7 — (no-op here);
 *   8,9 — enter a vehicle seat: validate the seat, evict an AI if needed, enter it, and on the client
 *          clear the player's prediction history; if the seat is full, try to AI-evict its occupant;
 *   10 — touch a device (door/control panel);
 *   else — melee/push a target object: stamp the target onto the unit, record the time, and compute
 *          which side the hit came from (front/back/left/right) for the target's flinch response.
 * On success, when the unit is locally simulated, the result is reported to the network. Returns whether
 * an action completed. */

#include <stdint.h>
#include "headers/real_matrix4x3.h"
#include "headers/real_vector3d.h"
#include "headers/simple_circular_queue.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/player_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/biped_datum.h"
#include "headers/biped_datum_state.h"
#include "headers/biped_datum_flags.h"
#include "headers/game_connection.h"
#include "headers/unit_animation_state.h"
#include "headers/object_type.h"
#include "headers/player_action_result.h"
#include "headers/unit_add_equipment_mode.h"
#include "headers/blam_data_globals.h"

#include "headers/network_game_client.h"
#include "headers/networked_set_action_result_type.h"
#include "headers/player_update_history.h"
extern void unit_drop_current_equipment(int unit_index);
extern uint8_t unit_add_equipment_to_inventory(int unit_index, int equipment_index, int16_t mode);
extern void hud_picked_up_powerup(int16_t local_player_index, int8_t machine_index, int powerup_definition_index);
extern int16_t game_connection(void);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t unit_can_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index, int *occupant_unit_index_reference);
extern uint8_t unit_enter_seat(int unit_index, int parent_unit_index, int16_t seat_index);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern void simple_circular_queue_empty(simple_circular_queue *queue);
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_clear(struct player_update_history *history);
extern uint8_t ai_try_vehicle_eviction(int actor_index, int requesting_unit_index, uint8_t actually_evict);
extern void device_touched(int device_index, int unit_index);
extern int game_time_get(void);
extern real_matrix4x3 *object_get_world_matrix(int object_index, real_matrix4x3 *matrix);
extern double __fabs(double x);
extern void player_set_action_result_to_network(int player_index, networked_set_action_result_type action_result_type, int action_object_index, int16_t action_result, int16_t action_seat_index, int weapon_to_drop_as_result_of_swap);

/* live object datum for an object handle */
static object_datum *object_data_ptr(int object_handle)
{
    return DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_handle)->datum;
}

uint8_t player_handle_action(int player_index)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);
    int   unit_index = player->unit_index;
    unit_datum *unit_object = (unit_datum *)object_data_ptr(unit_index);
    int16_t action_type = (uint16_t)player->action_result;
    char  can_enter = 0;
    int   result = 0;

    if ( action_type >= _player_action_result_swap_for_powerup
      && action_type <= _player_action_result_flip_vehicle )
    {
        if ( action_type == _player_action_result_swap_for_powerup )
        {
            unit_drop_current_equipment(unit_index);
            if ( unit_add_equipment_to_inventory(unit_index, player->action_object_index, _unit_add_equipment_normally) )
                hud_picked_up_powerup(player->local_player_index,
                                      player->network_player_data.machine_index,
                                      object_data_ptr(player->action_object_index)->definition_index);
            result = 1;
        }
        else if ( action_type != _player_action_result_swap_for_weapon && action_type != _player_action_result_add_weapon_to_inventory )
        {
            if ( action_type != _player_action_result_enter_vehicle && action_type != _player_action_result_evict_from_vehicle )
            {
                if ( action_type == _player_action_result_touch_device )
                {
                    device_touched(player->action_object_index, unit_index);
                }
                else
                {
                    /* melee/push target object */
                    int target_index = player->action_object_index;
                    biped_datum *target_object = (biped_datum *)object_data_ptr(target_index);
                    float forward_z = target_object->object.forward.n[2];
                    char side;

                    /* melee target/time reuse the unit's last-vehicle fields (DB member names) */
                    unit_object->unit.last_vehicle_index = target_index;
                    unit_object->unit.game_time_at_last_vehicle_exit = game_time_get();

                    if ( __fabs(forward_z) <= 0.7071067690849304 )
                    {
                        real_matrix4x3 target_matrix, unit_matrix;
                        float *target_pos = object_get_world_matrix(target_index, &target_matrix)->n[3];
                        real_matrix4x3 *unit_world = object_get_world_matrix(unit_index, &unit_matrix);
                        /* sign of (up x (target_pos - unit_pos)) . target_forward → left vs right */
                        float dx = target_pos[0] - unit_world->n[3][0];
                        float dy = target_pos[1] - unit_world->n[3][1];
                        float dz = target_pos[2] - unit_world->n[3][2];
                        float cross_x = (global_up3d->n[1] * dz) - (global_up3d->n[2] * dy);
                        float cross_y = (global_up3d->n[2] * dx) - (global_up3d->n[0] * dz);
                        float cross_z = (global_up3d->n[0] * dy) - (global_up3d->n[1] * dx);
                        float dot = (cross_x * target_object->object.forward.n[0])
                                  + ((target_object->object.forward.n[1] * cross_y)
                                          + (target_object->object.forward.n[2] * cross_z));
                        side = (dot > 0.0f) + 1;   /* 1 = left, 2 = right */
                    }
                    else
                    {
                        side = (forward_z < 0.0f) + 3;   /* 3 = front, 4 = back */
                    }

                    /* biped melee-flinch response (DB names): side in landing_recovery_time, state cleared */
                    target_object->biped.landing_recovery_time = side;
                    target_object->biped.state = biped_state_idle;
                    target_object->biped.flags |= (1u << _biped_movement_passes_through_bipeds_bit);
                }
                result = 1;
            }
            else
            {
                /* enter vehicle seat (action 8/9) */
                if ( game_connection() != _game_connection_network_client
                  || (unit_index != -1 && object_try_and_get_and_verify_type(unit_index, object_mask_unit)) )
                    can_enter = 1;

                if ( can_enter == 1 )
                {
                    int occupant[4];
                    occupant[0] = -1;

                    /* on the client, evict the AI currently boarding this seat */
                    if ( game_connection() == _game_connection_network_client
                      && !unit_can_enter_seat(unit_index, player->action_object_index,
                                              player->action_seat_index, occupant) )
                    {
                        unit_datum *vehicle = object_try_and_get_and_verify_type(unit_index, object_mask_unit);
                        if ( vehicle && vehicle->unit.animation.state == _unit_state_exiting_seat )
                            unit_exit_seat_end(unit_index, 1u, 1u, 0);
                    }

                    if ( unit_can_enter_seat(unit_index, player->action_object_index,
                                             player->action_seat_index, occupant) )
                    {
                        unit_enter_seat(unit_index, player->action_object_index, player->action_seat_index);
                        result = 1;
                        if ( game_connection() == _game_connection_network_client )
                        {
                            if ( (uint16_t)player->local_player_index == 0xFFFF )
                            {
                                simple_circular_queue_empty(&player->___u26.client_update_data.___u0.remote_player.position_queue.queue);
                                simple_circular_queue_empty(&player->___u26.client_update_data.___u0.remote_player.vehicle_update_queue.queue);
                            }
                            else if ( global_network_game_client_get() )
                            {
                                player_update_history_clear(0);
                            }
                        }
                    }
                    else if ( occupant[0] != -1 )
                    {
                        /* seat occupied: try to AI-evict the occupant's controlling actor */
                        int actor = ((unit_datum *)object_data_ptr(occupant[0]))->unit.actor_index;
                        if ( actor != -1 )
                        {
                            ai_try_vehicle_eviction(actor, unit_index, 1u);
                            result = 1;
                        }
                    }
                }
            }
        }
    }

    if ( (uint8_t)result == 1 && !unit_object->object.datum_role )
        player_set_action_result_to_network(player_index, _networked_action_result_type_action,
                                            player->action_object_index,
                                            (uint16_t)player->action_result,
                                            player->action_seat_index, -1);

    return result;
}
