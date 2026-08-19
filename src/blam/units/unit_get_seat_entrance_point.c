/* unit_get_seat_entrance_point @0x836C7058 — compute the world-space seat, entrance and enter-hint points for a
 * unit boarding a given seat of a parent vehicle. Resolves the entering unit's animation graph and model, and
 * the parent's seat definition (unit definition seats block). Matches the seat label against the animation
 * graph's unit_seats block to find the seat-boarding entry, takes its "enter" animation (index 7 of the seat's
 * animation list), and evaluates the animation's root transform relative to the seat marker. The entrance point
 * is the seat-marker transform composed with the animation root; the hint point is the "<seat marker>
 * enter-hint" marker. Returns 1 on success, 0 if the seat entry or its enter animation is absent. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/model.h"
#include "headers/animation.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/unit_seat_animation.h"
#include "headers/object_marker.h"
#include "headers/real_matrix4x3.h"
#include "headers/real_point3d.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/unit_seat.h"
#include "headers/blam_data_globals.h"

/* stricmp provided by CRT via <string.h>; local extern removed (C28251: the local redeclaration drops the header's annotations) */
extern int16_t object_get_marker_by_name(int object_index, const char *name, object_marker *markers, int16_t maximum_marker_count);
extern void animation_get_root_matrix(const model *model, const animation *animation, int16_t frame_index, real_matrix4x3 *matrix);
extern void matrix4x3_multiply(const real_matrix4x3 *a, const real_matrix4x3 *b, real_matrix4x3 *result);

uint8_t unit_get_seat_entrance_point(int unit_index, int parent_unit_index, int16_t seat_index,
                                     real_point3d *entrance_point, real_point3d *seat_point, real_point3d *hint_point)
{

    unit_definition *unit_tag =
        TAG_GET(unit_definition, ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum)->definition_index);
    animation_graph *graph = TAG_GET(animation_graph, unit_tag->object.animation_graph.index);
    const model *unit_model = TAG_GET(const model, unit_tag->object.model.index);
    unit_definition *parent_tag =
        TAG_GET(unit_definition, ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent_unit_index)->datum)->definition_index);
    unit_seat *seat_definition = &((unit_seat *)parent_tag->unit.seats.address)[seat_index];
    animation_graph_unit_seat *graph_seats = (animation_graph_unit_seat *)graph->unit_seats.address;

    /* Find the animation-graph unit seat whose label matches the seat name. */
    short seat_entry_index = -1;
    if ( graph->unit_seats.count > 0 )
    {
        int entry = 0;
        while ( stricmp(graph_seats[entry].label, seat_definition->label) )
        {
            entry = (short)(entry + 1);
            if ( entry >= graph->unit_seats.count )
                goto no_entry;
        }
        seat_entry_index = entry;
    }
no_entry:
    if ( seat_entry_index == -1 )
        return 0;

    animation_graph_unit_seat *graph_seat = &graph_seats[seat_entry_index];
    short enter_animation_index = graph_seat->animations.count <= _unit_seat_animation_seat_enter
        ? -1 : ((short *)graph_seat->animations.address)[_unit_seat_animation_seat_enter];
    if ( enter_animation_index == -1 )
        return 0;

    const animation *enter_animation = &((const animation *)graph->animations.address)[enter_animation_index];

    object_marker seat_marker;
    real_matrix4x3 animation_root;
    real_matrix4x3 entrance_matrix;
    object_get_marker_by_name(parent_unit_index, seat_definition->marker_name, &seat_marker, 1);
    animation_get_root_matrix(unit_model, enter_animation, 0, &animation_root);
    matrix4x3_multiply(&seat_marker.matrix, &animation_root, &entrance_matrix);

    char hint_marker_name[368];
    strcpy(hint_marker_name, seat_definition->marker_name);
    strcat(hint_marker_name, " enter-hint");

    object_marker hint_marker;
    object_get_marker_by_name(parent_unit_index, hint_marker_name, &hint_marker, 1);

    if ( seat_point )
    {
        seat_point->n[0] = seat_marker.matrix.n[3][0];
        seat_point->n[1] = seat_marker.matrix.n[3][1];
        seat_point->n[2] = seat_marker.matrix.n[3][2];
    }
    if ( entrance_point )
    {
        entrance_point->n[0] = entrance_matrix.n[3][0];
        entrance_point->n[1] = entrance_matrix.n[3][1];
        entrance_point->n[2] = entrance_matrix.n[3][2];
    }
    if ( hint_point )
    {
        hint_point->n[0] = hint_marker.matrix.n[3][0];
        hint_point->n[1] = hint_marker.matrix.n[3][1];
        hint_point->n[2] = hint_marker.matrix.n[3][2];
    }
    return 1;
}
