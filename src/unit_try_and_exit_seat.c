/* unit_try_and_exit_seat @0x836D3BF8 — attempt to make a seated unit leave its seat. Returns 1 if the exit
 * was started, 0 otherwise. Refuses on a network client unless should_allow_clients is set, and refuses if
 * the unit is not actually in a seat. A vehicle-type unit (object.type == object_type_vehicle) is ejected
 * via unit_exit_seat_end; otherwise, if no seat animation is already playing, the seat's exit animation is
 * chosen and started, the unit is made visible, the vehicle is told to set its exit-animation state, and the
 * AI/network are notified.
 */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/object_type.h"
#include "headers/game_connection.h"
#include "headers/unit_animation_state.h"
#include "headers/unit_seat_animation.h"
#include "headers/blam_data_globals.h"

extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t game_connection(void);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern uint8_t unit_animation_busy(unit_animation *animation);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void ai_handle_exit_vehicle(int unit_index, int vehicle_index);
extern void unit_exit_seat_to_network(int object_index, uint8_t force_exit);

#define animation_update_kind_affects_game_state 1

uint8_t unit_try_and_exit_seat(int object_index, uint8_t should_allow_clients)
{
    int started = 0;
    unit_datum *unit_data = object_try_and_get_and_verify_type(object_index, object_mask_unit);
    if ( !unit_data )
        return 0;
    unit_animation *animation = &unit_data->unit.animation;

    int vehicle_object_index = unit_data->object.parent_object_index;
    if ( (game_connection() != _game_connection_network_client || (game_connection() == _game_connection_network_client && should_allow_clients == 1))
      && vehicle_object_index != -1
      && (unsigned __int16)unit_data->unit.parent_seat_index != 0xFFFF )  /* in a seat */
    {
        if ( unit_data->object.type == object_type_vehicle )  /* vehicles exit immediately */
        {
            unit_exit_seat_end(object_index, 1u, should_allow_clients, 1u);
        }
        else if ( !unit_animation_busy(animation) )
        {
            unit_definition *definition = TAG_GET(unit_definition, unit_data->definition_index);
            int animation_graph_index = definition->object.animation_graph.index;
            animation_graph *graph = TAG_GET(animation_graph, animation_graph_index);
            const animation_graph_unit_seat *seat_animations =
                (const animation_graph_unit_seat *)graph->unit_seats.address + animation->seat_index;
            __int16 animation_index = seat_animations->animations.count <= _unit_seat_animation_seat_exit
                ? -1
                : ((const __int16 *)seat_animations->animations.address)[_unit_seat_animation_seat_exit];

            if ( animation_index != -1 )
            {
                unit_datum *vehicle_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, vehicle_object_index)->datum);
                if ( vehicle_data->unit.driver_object_index == object_index )  /* unit is the driver */
                    unit_animation_set_state(vehicle_object_index, _unit_state_opening);

                __int16 permutation = animation_choose_random_permutation_internal(
                    animation_update_kind_affects_game_state, animation_graph_index, animation_index);

                unit_datum *object_data = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum);
                object_data->object.animation.animation_graph_index = animation_graph_index;
                object_data->object.animation.state.index = permutation;
                object_data->object.animation.state.frame_index = 0;
                object_set_visibility(object_index, 1u);

                int vehicle_index = unit_data->object.parent_object_index;
                animation->state = _unit_state_exiting_seat;
                ai_handle_exit_vehicle(object_index, vehicle_index);
                started = 1;
            }
        }
    }

    if ( (unsigned __int8)started == 1 )
    {
        game_connection();
        if ( !unit_data->object.datum_role )  /* not a network-remote object */
            unit_exit_seat_to_network(object_index, 0);
    }
    return started;
}
