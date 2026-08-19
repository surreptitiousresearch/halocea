/* players_server_catchup_on_client_updates @0x836AB1E0 — server-side: when a remote player's buffered
 * action queue has backed up (more than 4 entries), drain it by replaying each queued action through
 * the unit-control / unit-update path so the server catches the simulation up to the client.
 *
 * For each non-local player whose action queue is backed up, repeatedly read the next action and, if
 * the unit is controllable, build a unit_control_data packet (either an "idle" packet built from the
 * unit's current orientation when input is inhibited, or one driven by the queued action) and apply it,
 * then advance the unit (or its vehicle, under the new vehicle-update scheme).
 *
 *
 * Deviation: the decompiler's _DWORD* return (the last iterator result) is discarded — void.
 * The piecewise __QWORD copies that set looking/facing from aiming are written as full vector copies. */

#include <stdint.h>
#include "headers/object_header_datum.h"
#include "headers/data_iterator.h"
#include "headers/action_queue.h"
#include "headers/unit_control_data.h"
#include "headers/players_globals.h"
#include "headers/player_datum.h"
#include "headers/unit_datum.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"
#include "headers/unit_desired_animation_state.h"


#include "headers/data_array.h"
#include "headers/simple_circular_queue.h"
#include "headers/real_vector3d.h"
#include "headers/real_euler_angles2d.h"
extern void data_iterator_new(data_iterator *iterator, data_array *data);
extern void *data_iterator_next(data_iterator *iterator);
extern void *datum_try_and_get(const data_array *data, int index);
extern action_queue *update_server_get_player_action_queue(int player_index);
extern int simple_circular_queue_get_number_of_queued_entries(const simple_circular_queue *queue);
extern uint8_t action_queue_read(action_queue *queue, player_action *action, int *completed_client_update_id);
extern uint8_t unit_controllable(int unit_index);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t weapon_must_be_readied(int weapon_index);
extern void player_aiming_vector_from_facing(int player_index, real_vector3d *aiming_vector, const real_euler_angles2d *desired_facing);
extern void unit_control(int unit_index, const unit_control_data *control_data, int completed_client_update_id);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern uint8_t unit_seat_is_driver(int vehicle_index, int16_t seat_index);
extern int object_update(int object_index);
extern uint8_t unit_update(int unit_index);
extern uint8_t biped_update(int biped_index);

void players_server_catchup_on_client_updates(void)
{
    data_iterator iter;
    player_datum *player;
    int scratch[4];
    player_action action;
    unit_control_data control;

    data_iterator_new(&iter, player_data);
    for ( player = data_iterator_next(&iter); player;
          player = data_iterator_next(&iter) )
    {
        action_queue *queue;
        if ( (uint16_t)player->local_player_index != 0xFFFF )
            continue;
        queue = update_server_get_player_action_queue(iter.index);
        /* binary re-queries the count twice at entry; first result is discarded server-side
         * (twin of the client's captured number_of_queued_entries) */
        simple_circular_queue_get_number_of_queued_entries(&queue->queue);
        if ( simple_circular_queue_get_number_of_queued_entries(&queue->queue) <= 4 )
            continue;

        do
        {
            int unit_index;
            unit_datum *unit;

            action_queue_read(queue, &action, scratch);
            unit_index = player->unit_index;
            if ( unit_index == -1 || !unit_controllable(unit_index) )
                continue;

            unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

            if ( players_globals->input_inhibited )
            {
                /* skip control entirely if the unit is in a seat/vehicle */
                if ( unit->unit.swarm_actor_index != -1 || unit->unit.actor_index != -1 )
                    goto advance;

                control.animation_state = _unit_animation_state_in_combat;
                control.aiming_speed    = 0;
                control.control_flags   = 0;
                control.weapon_index    = -1;
                /* recovered: *(_DWORD *)&control.grenade_index = -1 -> grenade_index + zoom_level */
                control.grenade_index   = -1;
                control.zoom_level      = -1;
                control.primary_trigger = 0.0f;
                control.facing_vector  = unit->unit.desired_facing_vector;
                control.aiming_vector  = unit->unit.desired_aiming_vector;
                control.looking_vector = unit->unit.desired_looking_vector;
                control.throttle = *global_zero_vector3d;
            }
            else
            {
                int weapon = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_index);
                if ( weapon != -1 && weapon_must_be_readied(weapon) )
                    action.desired_weapon_index = unit->unit.current_weapon_index;

                control.control_flags = action.control_flags;
                player_aiming_vector_from_facing(iter.index, &control.aiming_vector, &action.desired_facing);
                control.throttle.n[2]  = 0.0f;
                control.looking_vector = control.aiming_vector;   /* piecewise qword copies in original */
                control.facing_vector  = control.aiming_vector;
                control.throttle.n[0]  = action.throttle.n[0];
                control.throttle.n[1]  = action.throttle.n[1];
                control.primary_trigger = action.primary_trigger;
                control.animation_state = _unit_animation_state_in_combat;
                control.aiming_speed    = 0;
                /* recovered: *(_DWORD *)&control.grenade_index = *(_DWORD *)&action.desired_grenade_index -> grenade_index + zoom_level */
                control.grenade_index   = action.desired_grenade_index;
                control.zoom_level      = action.desired_zoom_level;
                control.weapon_index    = action.desired_weapon_index;
            }

            unit_control(unit_index, &control, scratch[0]);

advance:
            {
                uint8_t is_driver = 0;
                player_datum *p = datum_try_and_get(player_data, iter.index);
                if ( p )
                {
                    unit_datum *u = object_try_and_get_and_verify_type(p->unit_index, object_mask_unit);
                    if ( u && u->object.parent_object_index != -1 )
                        is_driver = unit_seat_is_driver(u->object.parent_object_index, u->unit.parent_seat_index);
                }
                if ( is_driver && use_new_vehicle_update_scheme )
                {
                    object_update(unit->object.parent_object_index);
                }
                else
                {
                    unit_update(player->unit_index);
                    biped_update(player->unit_index);
                }
            }
        }
        while ( simple_circular_queue_get_number_of_queued_entries(&queue->queue) > 4 );
    }
}
