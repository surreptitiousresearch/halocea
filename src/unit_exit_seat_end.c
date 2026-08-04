/* unit_exit_seat_end @0x836D2D70 — finish a unit's exit from a vehicle seat: detach it from the vehicle,
 * reposition and re-orient it at the seat's exit marker, restore its visibility and node matrices, run the
 * biped exit-seat finishing, and update driver/gunner/animation state. On the host it also clears the
 * exiting player's update queues and, when appropriate, replicates the exit to clients; on a client it clears
 * the local player's prediction history.
 *
 * Most of the work is gated on the unit still occupying a seat (unit.parent_seat_index != -1) of a real
 * parent (object.parent_object_index != -1). The seat's exit transform comes from the unit model's nodes
 * block; the landing position is the unit's current origin shifted by the marker-space offset between its
 * root node and the seat marker. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_orientation.h"
#include "headers/real_vector3d.h"
#include "headers/real_point3d.h"
#include "headers/object_marker.h"
#include "headers/unit_animation_update_data.h"
#include "headers/simple_circular_queue.h"
#include "headers/unit_datum.h"
#include "headers/vehicle_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/_object_definition.h"
#include "headers/model.h"
#include "headers/player_datum.h"
#include "headers/game_connection.h"
#include "headers/unit_animation_state.h"
#include "headers/base_seat.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"

#include "headers/network_game_client.h"
extern int16_t game_connection(void);
extern int game_time_get(void);
extern real_matrix4x3 *object_get_node_matrix(int object_index, int16_t node_index);
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern real_vector3d *matrix4x3_inverse_transform_vector(const real_matrix4x3 *matrix, const real_vector3d *vector, real_vector3d *result);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern void object_detach(int child_object_index);
extern void object_set_position(int object_index, const real_point3d *position, const real_vector3d *forward, const real_vector3d *up);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void unit_update_driver_and_gunner(int parent_object_index);
extern void unit_adjust_for_seat_change(int unit_index);
extern int16_t unit_update_animation(int unit_index, unit_animation_update_data *data);
extern void biped_exit_seat_end(int biped_index, int old_vehicle_index);
extern void object_compute_node_matrices_recursive(int object_index);
extern uint8_t unit_seats_empty(int unit_index);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern void *datum_try_and_get(const data_array *data, int index);
extern void simple_circular_queue_empty(simple_circular_queue *queue);
extern void unit_exit_seat_to_network(int object_index, uint8_t force_exit);
extern network_game_client *global_network_game_client_get(void);
extern void player_update_history_clear(struct player_update_history *history);

void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit,
                        uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate)
{
    if ( game_connection() == _game_connection_network_client && should_allow_clients != 1 )
        return;

    unit_datum *object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
    int parent_object_index = object_data->object.parent_object_index;
    if ( parent_object_index != -1 )
    {
        __int16 seat_index = object_data->unit.parent_seat_index;
        if ( seat_index != -1 )
        {
            unit_datum *parent_data =
                ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_object_index)->datum);
            unit_definition *parent_definition = TAG_GET(unit_definition, parent_data->definition_index);
            unit_seat *seat_definition = (unit_seat *)parent_definition->unit.seats.address + seat_index;

            real_matrix4x3 *node_matrix = object_get_node_matrix(object_index, 0);

            object_marker markers[2];
            object_get_marker_by_name(parent_object_index, seat_definition->marker_name, markers, 1);

            real_vector3d marker_offset;
            marker_offset.n[0] = node_matrix->n[3][0] - markers[0].matrix.n[3][0];
            marker_offset.n[1] = node_matrix->n[3][1] - markers[0].matrix.n[3][1];
            marker_offset.n[2] = node_matrix->n[3][2] - markers[0].matrix.n[3][2];

            real_vector3d marker_local_offset[2];
            matrix4x3_inverse_transform_vector(&markers[0].matrix, &marker_offset, marker_local_offset);

            int vehicle_primary_occupant = parent_data->unit.driver_object_index;

            _object_definition *own_definition = TAG_GET(_object_definition, object_data->definition_index);
            int model_tag = own_definition->model.index;
            model *model_definition = TAG_GET(model, model_tag);
            /* root node's default transform, read matrix-shaped from the nodes block (faithful to binary) */
            const real_matrix4x3 *exit_transform = (const real_matrix4x3 *)model_definition->nodes.address;
            const real_matrix4x3 *exit_orientation = exit_transform + 2;
            float exit_z = exit_transform->n[3][2];
            float exit_x = exit_transform->n[3][0];
            float exit_y = exit_transform->n[3][1];
            float exit_drop = exit_z;

            if ( vehicle_primary_occupant == object_index && parent_data->unit.animation.state != _unit_state_opening )
            {
                int parent = object_data->object.parent_object_index;
                if ( parent != -1 )
                    unit_animation_set_state(parent, _unit_state_opening);
            }

            object_data->unit.last_vehicle_index = parent_object_index;
            int exit_time = game_time_get();
            int own_primary = object_data->unit.driver_object_index;
            object_data->unit.game_time_at_last_vehicle_exit = exit_time;
            if ( own_primary == object_index )
                object_data->unit.driver_object_index = -1;
            if ( object_data->unit.gunner_object_index == object_index )
                object_data->unit.gunner_object_index = -1;

            object_detach(object_index);

            real_point3d landing_position;
            landing_position.n[0] = object_data->object.position.n[0] + marker_offset.n[0];
            landing_position.n[1] = object_data->object.position.n[1] + marker_offset.n[1];
            landing_position.n[2] = (object_data->object.position.n[2] + marker_offset.n[2]) - exit_drop;
            object_set_position(object_index, &landing_position, 0, 0);

            real_matrix4x3 oriented;
            matrix4x3_multiply(object_get_node_matrix(object_index, 0), exit_orientation, &oriented);
            object_data->object.forward.n[0] = oriented.n[0][0];
            object_data->object.forward.n[1] = oriented.n[0][1];
            object_data->object.forward.n[2] = oriented.n[0][2];
            object_data->object.up.n[0] = oriented.n[2][0];
            object_data->object.up.n[1] = oriented.n[2][1];
            object_data->object.up.n[2] = oriented.n[2][2];

            object_set_visibility(object_index, 1u);
            object_data->unit.parent_seat_index = -1;
            object_data->unit.animation.base_seat_index = _base_seat_stand;

            if ( parent_data->unit.driver_object_index == object_index )
                parent_data->unit.driver_object_index = -1;
            if ( parent_data->unit.gunner_object_index == object_index )
                parent_data->unit.gunner_object_index = -1;

            unit_update_driver_and_gunner(parent_object_index);
            unit_adjust_for_seat_change(object_index);

            unit_animation_update_data animation_update;
            animation_update.state_desired = _unit_state_airborne;
            animation_update.crouching = 0;
            unit_update_animation(object_index, &animation_update);

            /* root node real_orientation record in the object header block region (translation @0x10) */
            real_orientation *root_orientation =
                (real_orientation *)((char *)object_data + object_data->object.original_node_orientations.offset);
            root_orientation->translation.n[0] = exit_x;
            root_orientation->translation.n[1] = exit_y;
            root_orientation->translation.n[2] = exit_z;

            if ( !object_data->object.type )
                biped_exit_seat_end(object_index, parent_object_index);
            object_compute_node_matrices_recursive(object_index);

            if ( unit_seats_empty(parent_object_index) == 1 )
            {
                vehicle_datum *vehicle = object_try_and_get_and_verify_type(parent_object_index, object_mask_vehicle);
                if ( vehicle )
                    vehicle->vehicle.last_controlled_time = game_time_get();
            }

            if ( game_connection() == _game_connection_network_client )
            {
                player_datum *player = datum_try_and_get(player_data, object_data->unit.player_index);
                if ( player && (unsigned __int16)player->local_player_index == 0xFFFF )
                {
                    simple_circular_queue_empty(
                        &player->___u26.client_update_data.___u0.remote_player.position_queue.queue);
                    simple_circular_queue_empty(
                        &player->___u26.client_update_data.___u0.remote_player.vehicle_update_queue.queue);
                }
            }
        }
    }

    if ( !is_part_of_non_forced_exit && should_replicate_if_appropriate == 1 && !(int)object_data->object.datum_role )
        unit_exit_seat_to_network(object_index, 1u);

    if ( game_connection() == _game_connection_network_client )
    {
        player_datum *player = datum_try_and_get(player_data, object_data->unit.player_index);
        if ( player && (unsigned __int16)player->local_player_index != 0xFFFF )
        {
            if ( global_network_game_client_get() )
                player_update_history_clear(0);
        }
    }
}
