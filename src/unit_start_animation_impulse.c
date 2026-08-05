/* unit_start_animation_impulse @0x836CEB78 — start playing an AI animation impulse on a unit (if it can). Looks
 * up the impulse's animation in the unit's current animation graph, picks a random permutation, stores it as
 * the object's active animation (object.animation graph/index/frame), flags the unit animation control block
 * busy (unit.animation.flags bit 0x1, unit.animation.state = _unit_state_ai_impulse) and, for a free-standing (unparented,
 * non-vehicle) unit, aligns its facing to the supplied 2D vector.
 *
 * Deviation: Hex-Rays collapsed the animation/definition pointer derivation into undefined locals (v8 = unit
 * definition tag, v9 = the selected animation entry); reconstructed from disassembly (same tag walk as
 * unit_test_animation_impulse: def -> animation graph -> unit_seats block -> seat by animation.seat_index ->
 * weapon_classes block -> class by animation.weapon_index; that class's animations block is the impulse
 * table). */

#include <stdint.h>
#include "headers/global_tag_instances.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/real_vector2d.h"
#include "headers/unit_datum.h"
#include "headers/unit_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_unit_seat.h"
#include "headers/animation_graph_weapon_class.h"
#include "headers/unit_animation_flags.h"
#include "headers/unit_animation_state.h"
#include "headers/blam_data_globals.h"

extern int animation_update_kind_affects_game_state;  /* animation_update_kind enum value */
extern uint8_t unit_can_play_animation_impulse(int unit_index, int16_t animation_impulse);
extern int16_t unit_animation_impulse_get_index(int16_t animation_impulse, int16_t *interpolation_frame_count);
extern void object_start_interpolation(int object_index, int16_t frame_count);
extern int16_t animation_choose_random_permutation_internal(int render_or_affects_game_state, int animation_graph_index, int16_t animation_index);
extern void unit_align_facing(int unit_index, const real_vector2d *alignment_vector);

/* renamed from tag_data() to avoid clash with the tag_data typedef */
static void *tag_get_data(int tag_index)
{
    return TAG_GET(void, tag_index);
}

int unit_start_animation_impulse(int unit_index, int16_t animation_impulse, real_vector2d *alignment_vector)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    if ( !unit_can_play_animation_impulse(unit_index, animation_impulse) )
        return 0;

    unit_definition *definition = (unit_definition *)tag_get_data(unit->definition_index);
    /* DEVIATION: disasm is a full lwz of the tag handle (+0x44) then clrlslwi (low-half mask ×32),
     * i.e. exactly the TAG_GET resolve — the decompiler's halfword pun was a misrendering */
    int graph_index = definition->object.animation_graph.index;
    animation_graph *graph = (animation_graph *)tag_get_data(graph_index);
    animation_graph_unit_seat *seat =
        &((animation_graph_unit_seat *)graph->unit_seats.address)[(signed char)unit->unit.animation.seat_index];
    animation_graph_weapon_class *weapon_class =
        &((animation_graph_weapon_class *)seat->weapon_classes.address)[(signed char)unit->unit.animation.weapon_index];

    int16_t interpolation_frame_count;
    int16_t index = unit_animation_impulse_get_index(animation_impulse, &interpolation_frame_count);
    int16_t animation_index;
    if ( index < 0 || index >= weapon_class->animations.count )
        animation_index = -1;
    else
        animation_index = ((int16_t *)weapon_class->animations.address)[index];
    if ( animation_index == -1 )
        return 0;

    object_start_interpolation(unit_index, interpolation_frame_count);
    int16_t permutation = animation_choose_random_permutation_internal(
        animation_update_kind_affects_game_state, definition->object.animation_graph.index, animation_index);

    /* decompiler refetched the same object-header slot */
    unit_datum *unit2 = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    unit2->object.animation.animation_graph_index = definition->object.animation_graph.index;
    unit2->object.animation.state.index = permutation;
    unit2->object.animation.state.frame_index = 0;

    int16_t anim_flags = unit->unit.animation.flags;
    unit->unit.animation.state = _unit_state_ai_impulse;
    unit->unit.animation.flags = anim_flags | (1u << _unit_animation_postpone_weapon_ik_until_interpolation_ends_bit);

    if ( alignment_vector && !unit->object.type && unit->object.parent_object_index == -1 )
        unit_align_facing(unit_index, alignment_vector);
    return 1;
}
