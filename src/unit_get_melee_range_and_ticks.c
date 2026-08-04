/* unit_get_melee_range_and_ticks @0x836C72D8 — for a unit's melee (or airborne-melee) animation, report the
 * weapon-reach x-offsets and the tick window. Resolves the unit's animation graph (unit tag word 17 →
 * animation-graph tag), selects the unit-animation block by the unit's animation state (stance byte +0x2A0,
 * sub-state byte +0x2A1), looks up the melee animation type (39 grounded, 42 airborne) in that block's index
 * table, then reads the animation's start/end reach via animation_get_x_offsets and its private-key-frame and
 * total-frame counts. Returns 1 if a melee animation exists, 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/_object_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/global_tag_instances.h"
#include "headers/animation.h"
#include "headers/weapon_class_animation.h"
#include "headers/blam_data_globals.h"

extern void animation_get_x_offsets(animation *animation, float *key_frame_offset_reference, float *last_frame_offset_reference);

uint8_t unit_get_melee_range_and_ticks(int unit_index, uint8_t airborne, int16_t *start_tick_count, float *start_range, int16_t *end_tick_count, float *end_range)
{
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int melee_animation_type = airborne == 0 ? _weapon_class_animation_melee : _weapon_class_animation_melee_airborne;

    _object_definition *unit_definition = TAG_GET(_object_definition, unit->definition_index);
    animation_graph *graph_tag = TAG_GET(animation_graph, unit_definition->animation_graph.index);

    animation_graph_unit_seat *seat =
        &((animation_graph_unit_seat *)graph_tag->unit_seats.address)[unit->unit.animation.seat_index];
    animation_graph_weapon_class *unit_animation =
        &((animation_graph_weapon_class *)seat->weapon_classes.address)[unit->unit.animation.weapon_index];

    short animation_index;
    if ( melee_animation_type < 0 || melee_animation_type >= unit_animation->animations.count )
        animation_index = -1;
    else
        animation_index = ((short *)unit_animation->animations.address)[melee_animation_type];

    if ( animation_index != -1 )
    {
        animation *melee_animation = &((animation *)graph_tag->animations.address)[animation_index];
        animation_get_x_offsets(melee_animation, start_range, end_range);
        if ( start_tick_count )
            *start_tick_count = melee_animation->private_key_frame_index;
        if ( end_tick_count )
            *end_tick_count = melee_animation->frame_count;
        return 1;
    }
    return 0;
}
