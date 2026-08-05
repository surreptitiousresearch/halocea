/* vehicle_scripting_unload @0x836D61? — script command to make AI units exit a vehicle. Every unit parented
 * to the given vehicle whose seat label matches seat_substring_name (an empty/NULL substring matches every
 * seat) is ejected. Player-controlled exits are left to the netcode; a vehicle-type occupant is exited
 * immediately, while a normal rider plays its seat-exit animation: the exit animation is looked up in the
 * unit's animation graph, the vehicle's controlled occupant is flagged to play the matching state, the rider's
 * animation block is primed, it is made visible, and AI exit handling is invoked. Returns the number of riders
 * that began an animated exit.
 *
 * DEVIATION 1: the decompiler coalesced the unit_index argument with the "match-all" byte flag into one local;
 * the disassembly shows unit_animation_set_state(unit_index, _unit_state_opening) takes the vehicle's own index.
 * DEVIATION 2: two game_connection() results are discarded by the decompiler (a lost comparison around the
 * client/host split); reproduced as bare calls. */

#include <stdint.h>
#include "headers/vehicle_datum.h"
#include "headers/vehicle_definition.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/object_iterator.h"
#include "headers/object_type.h"
#include "headers/game_connection.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"

typedef struct unit_animation unit_animation;

extern int animation_update_kind_affects_game_state;

extern void object_iterator_new(object_iterator *iterator, uint32_t type_flags, uint8_t flags);
extern void *object_iterator_next(object_iterator *iterator);
extern char *strlwr(char *string);
extern char *strstr(const char *haystack, const char *needle);
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t game_connection(void);
extern void unit_exit_seat_end(int object_index, uint8_t is_part_of_non_forced_exit, uint8_t should_allow_clients, uint8_t should_replicate_if_appropriate);
extern uint8_t unit_animation_busy(unit_animation *animation);
extern uint8_t unit_animation_set_state(int unit_index, int16_t new_state);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern void object_set_visibility(int object_index, uint8_t visible_flag);
extern void ai_handle_exit_vehicle(int unit_index, int vehicle_index);
extern void unit_exit_seat_to_network(int object_index, uint8_t force_exit);

int16_t vehicle_scripting_unload(int unit_index, const char *seat_substring_name)
{
    int16_t count = 0;
    if (unit_index == -1)
        return count;

    vehicle_datum *vehicle_object = ((vehicle_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    vehicle_definition *vehicle_def = TAG_GET(vehicle_definition, vehicle_object->definition_index);

    uint8_t match_all = (!seat_substring_name || seat_substring_name[0] == '\0');

    object_iterator iterator;
    object_iterator_new(&iterator, object_mask_unit, 0); /* units */
    for (int16_t *occupant = object_iterator_next(&iterator);
         occupant;
         occupant = object_iterator_next(&iterator))
    {
        if (((unit_datum *)occupant)->object.parent_object_index != unit_index)
            continue;

        char seat_label[384];
        const char *source = ((unit_seat *)vehicle_def->unit.seats.address
                              + ((unit_datum *)occupant)->unit.parent_seat_index)->label;
        char *destination = seat_label;
        do
        {
            *destination = *source++;
        }
        while (*destination++);
        strlwr(seat_label);

        if (!match_all && !strstr(seat_label, seat_substring_name))
            continue;

        uint8_t ejected = 0;
        int index = iterator.index;
        unit_datum *occupant_object = object_try_and_get_and_verify_type(index, object_mask_unit);
        if (occupant_object)
        {
            if (game_connection() == _game_connection_network_client) /* client */
            {
                game_connection();
            }
            else if (occupant_object->object.parent_object_index != -1
                     && (uint16_t)occupant_object->unit.parent_seat_index != 0xFFFF) /* seated */
            {
                if ((uint16_t)occupant_object->object.type == object_type_vehicle) /* vehicle-type occupant */
                {
                    unit_exit_seat_end(index, 1, 0, 1);
                }
                else if (!unit_animation_busy(&occupant_object->unit.animation))
                {
                    unit_definition *occupant_definition = TAG_GET(unit_definition, occupant_object->definition_index);
                    int animation_graph_index = occupant_definition->object.animation_graph.index;
                    animation_graph_unit_seat *seat_animations =
                        (animation_graph_unit_seat *)(TAG_GET(animation_graph, animation_graph_index))->unit_seats.address
                        + (unsigned char)occupant_object->unit.animation.seat_index;
                    int16_t animation_index = (seat_animations->animations.count <= 8)
                        ? -1
                        : ((uint16_t *)seat_animations->animations.address)[8];

                    if (animation_index != -1)
                    {
                        if (vehicle_object->unit.driver_object_index == index)
                            unit_animation_set_state(unit_index, _unit_state_opening);

                        int16_t permutation = animation_choose_random_permutation_internal(
                            animation_update_kind_affects_game_state, animation_graph_index, animation_index);

                        occupant_object->object.animation.animation_graph_index = animation_graph_index;
                        occupant_object->object.animation.state.index = permutation;
                        occupant_object->object.animation.state.frame_index = 0;
                        object_set_visibility(index, 1);
                        occupant_object->unit.animation.state = _unit_state_exiting_seat;
                        ai_handle_exit_vehicle(index, occupant_object->object.parent_object_index);
                        ejected = 1;
                    }
                }
            }

            if (ejected == 1)
            {
                game_connection();
                if (occupant_object->object.datum_role == _networked_datum_master)
                    unit_exit_seat_to_network(index, 0);
            }

            if (ejected)
                count = (int16_t)(count + 1);
        }
    }

    return count;
}
