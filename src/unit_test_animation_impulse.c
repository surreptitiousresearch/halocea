/* unit_test_animation_impulse @0x836C9728 — true if the unit can currently play the given animation impulse
 * AND its current animation-graph animation block actually defines an animation for that impulse's index.
 *
 * Deviation: Hex-Rays collapsed the animation-pointer derivation into an undefined local (v3); reconstructed
 * here from disassembly. The walk: unit definition (tag) -> animation graph (tag ref) -> unit_seats block ->
 * the seat picked by the unit's animation seat_index -> its weapon_classes block -> the class picked by the
 * animation weapon_index; that class's animations tag_block is the impulse table. */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/blam_data_globals.h"

extern uint8_t unit_can_play_animation_impulse(int unit_index, int16_t animation_impulse);
extern int16_t unit_animation_impulse_get_index(int16_t animation_impulse, int16_t *interpolation_frame_count);

/* renamed from tag_data() to avoid clash with the tag_data typedef */
static void *tag_get_data(int tag_index)
{
    return TAG_GET(void, tag_index);
}

uint8_t unit_test_animation_impulse(int unit_index, int16_t animation_impulse)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    if ( !unit_can_play_animation_impulse(unit_index, animation_impulse) )
        return 0;

    unit_definition *definition = (unit_definition *)tag_get_data(unit->definition_index);
    /* DEVIATION: full tag handle read; TAG_GET applies the low-half mask (see unit_start_animation_impulse) */
    int graph_index = definition->object.animation_graph.index;
    animation_graph *graph = (animation_graph *)tag_get_data(graph_index);
    animation_graph_unit_seat *seat =
        &((animation_graph_unit_seat *)graph->unit_seats.address)[(signed char)unit->unit.animation.seat_index];
    animation_graph_weapon_class *weapon_class =
        &((animation_graph_weapon_class *)seat->weapon_classes.address)[(signed char)unit->unit.animation.weapon_index];

    int16_t interpolation_frame_count;
    int16_t index = unit_animation_impulse_get_index(animation_impulse, &interpolation_frame_count);
    return index >= 0
        && index < weapon_class->animations.count
        && ((int16_t *)weapon_class->animations.address)[index] != -1;
}
