/* unit_set_or_test_seat_and_weapon_label @0x836CDEB8 — search a unit's model for a seat / weapon / animation
 * label triple. seat_label and weapon_label are optional filters (null = match any). Returns 1 if a matching
 * (seat, weapon, animation-label) combination exists. When change_flag is set, the matching indices are
 * latched into the unit's animation-control fields (unit.animation) and the "extra occupants" flag
 * (bit 2 of unit.animation.flags) is updated, so the unit will play the corresponding seated/weapon animation.
 *
 * NOTE: the tables walked here are the animation graph's unit_seats / weapon_classes / weapon_types blocks
 * (the "model" in the original comment was the animation graph: object def dword[17] is
 * object.animation_graph.index). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/animation_graph_weapon_type.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"

extern int  stricmp(const char *a, const char *b);
extern int  strcmp(const char *a, const char *b);
extern int16_t seat_label_to_base_seat_index(const char *seat_label);

uint8_t unit_set_or_test_seat_and_weapon_label(int object_index, const char *seat_label,
                                           const char *weapon_label, uint8_t change_flag)
{
    int found = 0;
    int seat_index = 0;

    unit_datum *object_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, object_index)->datum;
    unit_definition *object_definition = TAG_GET(unit_definition, object_data->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, object_definition->object.animation_graph.index);

    int seat_count = graph->unit_seats.count;
    for ( int seat_loop = 0; seat_loop < seat_count; seat_loop = (int16_t)(seat_loop + 1) )
    {
        seat_index = seat_loop;
        animation_graph_unit_seat *seat =
            &((animation_graph_unit_seat *)graph->unit_seats.address)[seat_loop];
        if ( seat_label && stricmp(seat_label, seat->label) )
            continue;

        int weapon_index = 0;
        int weapon_count = seat->weapon_classes.count;
        for ( int weapon_loop = 0; weapon_loop < weapon_count; weapon_loop = (int16_t)(weapon_loop + 1) )
        {
            weapon_index = weapon_loop;
            animation_graph_weapon_class *weapon =
                &((animation_graph_weapon_class *)seat->weapon_classes.address)[weapon_loop];
            int label_index = 0;
            int label_count = weapon->weapon_types.count;
            if ( label_count <= 0 )
                continue;

            int matched = 0;
            for ( int label_loop = 0; ; )
            {
                const char *label =
                    ((animation_graph_weapon_type *)weapon->weapon_types.address)[label_loop].label;
                if ( !weapon_label )
                {
                    label_index = label_loop;
                    matched = 1;
                    break;
                }
                /* weapon_label vs label, with "unarmed" (case-sensitive, per the inlined strcmp loop)
                 * treated as matching the empty label */
                if ( (!strcmp(weapon_label, "unarmed") && !*label) || !stricmp(weapon_label, label) )
                {
                    label_index = label_loop;
                    matched = 1;
                    break;
                }
                label_loop = (int16_t)(label_loop + 1);
                if ( label_loop >= weapon->weapon_types.count )
                    break;
            }

            if ( !matched )
                continue;

            if ( change_flag )
            {
                int seat_animation_count = seat->animations.count;
                uint16_t *seat_animations = (uint16_t *)seat->animations.address;
                int has_extra_occupants =
                       (seat_animation_count > 2 && seat_animations[2] != 0xFFFF)
                    || (seat_animation_count > 3 && seat_animations[3] != 0xFFFF)
                    || (seat_animation_count > 4 && seat_animations[4] != 0xFFFF);

                if ( (uint8_t)object_data->unit.animation.state != _unit_state_user_animation )
                    object_data->unit.animation.state = -1;
                object_data->unit.animation.seat_index = seat_index;
                object_data->unit.animation.base_seat_index = seat_label_to_base_seat_index(seat_label);
                object_data->unit.animation.weapon_index = weapon_index;
                object_data->unit.animation.weapon_type_index = label_index;
                if ( has_extra_occupants )
                    object_data->unit.animation.flags |= (1u << _unit_animation_showing_acceleration_bit);
                else
                    object_data->unit.animation.flags &= ~(1u << _unit_animation_showing_acceleration_bit);
            }
            found = 1;
        }
    }

    return found;
}
