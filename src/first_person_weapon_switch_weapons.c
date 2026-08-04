/* first_person_weapon_switch_weapons @0x8369FDD8 — rebind a local player's first-person viewmodel to the
 * weapon now in the unit's current weapon slot. Hides the old viewmodel, looks up the new weapon, validates
 * that its first-person model + animation graph exist and that the node-remapping tables (for both the
 * weapon model and the shared first-person "hands" model) can be built, then commits the new weapon,
 * resets all viewmodel animation/sound state, drives it to state 0, and restores visibility.
 *
 * The weapon definition's interface block names the first-person model and animation graph. The overlay
 * (idle) animation is animation index [4] in the graph's first first-person animation set, accepted only
 * if its frame count is >= 9. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/game_globals_tag.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_first_person_weapon_animations.h"
#include "headers/animation.h"
#include "headers/game_globals_first_person_interface.h"
#include "headers/first_person_weapon_state.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/blam_data_globals.h"


extern void first_person_weapon_set_visibility(int16_t local_player_index, uint8_t visible);
extern int unit_inventory_get_weapon(int unit_index, int16_t index);
extern uint8_t model_build_remapping_table_for_animation_graph(int model_index, int animation_graph_index, int16_t *model_node_indices_to_animation_node_indices);
extern void first_person_weapon_set_state(int16_t local_player_index, int16_t new_state, uint8_t reset_sounds);
extern void first_person_weapon_predict(int16_t local_player_index);

void first_person_weapon_switch_weapons(int16_t local_player_index)
{
    int visible = first_person_weapons[local_player_index].visible;
    first_person_weapon *fp_weapon = &first_person_weapons[local_player_index];
    fp_weapon->weapon_index = -1;
    if ( visible )
        first_person_weapon_set_visibility(local_player_index, 0);

    int unit_index = fp_weapon->unit_index;
    if ( unit_index == -1 )
        goto predict;

    unit_datum *unit_data = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, fp_weapon->unit_index)->datum;
    int weapon_index = unit_inventory_get_weapon(unit_index, unit_data->unit.current_weapon_index);
    if ( weapon_index == -1 )
        goto predict;

    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    if ( definition->weapon.interface_definition.first_person_model.index == -1 )   /* no first-person model */
        goto predict;

    int first_person_animation_graph = definition->weapon.interface_definition.first_person_animations.index;
    if ( first_person_animation_graph == -1 )
        goto predict;

    animation_graph *graph = TAG_GET(animation_graph, first_person_animation_graph);
    if ( !graph->first_person_weapon_animations.count )
        goto predict;
    animation_graph_first_person_weapon_animations *animation_set =
        (animation_graph_first_person_weapon_animations *)graph->first_person_weapon_animations.address;
    if ( !animation_set )
        goto predict;

    fp_weapon->overlay_animation_index = -1;
    __int16 overlay_animation;
    if ( animation_set->animations.count <= _first_person_weapon_animation_overlays )
        overlay_animation = -1;
    else
        overlay_animation = ((__int16 *)animation_set->animations.address)[_first_person_weapon_animation_overlays];
    if ( overlay_animation != -1
      && ((animation *)graph->animations.address)[overlay_animation].frame_count >= 9 )
        fp_weapon->overlay_animation_index = overlay_animation;

    int hands_model = ((game_globals_first_person_interface *)global_game_globals->first_person_interface.address)->hands.index;
    if ( hands_model != -1 )
        fp_weapon->hands_node_remapping_table_valid = model_build_remapping_table_for_animation_graph(
            hands_model, first_person_animation_graph, fp_weapon->hands_node_remapping_table);

    unsigned __int8 weapon_table_valid = model_build_remapping_table_for_animation_graph(
        definition->weapon.interface_definition.first_person_model.index, first_person_animation_graph,
        fp_weapon->weapon_node_remapping_table);
    fp_weapon->weapon_node_remapping_table_valid = weapon_table_valid;

    if ( weapon_table_valid && fp_weapon->hands_node_remapping_table_valid )
    {
        fp_weapon->weapon_index = weapon_index;
        fp_weapon->state = -1;
        fp_weapon->state_animation.index = -1;
        fp_weapon->moving_animation.index = -1;
        fp_weapon->overcharged_jitter_animation.index = -1;
        fp_weapon->ticks_idle = 0;
        fp_weapon->firing_push_back = 0.0f;
        fp_weapon->current_sound_index = -1;
        fp_weapon->firing_push_back_velocity = 0.0f;
        fp_weapon->current_sound_state = -1;
        first_person_weapon_set_state(local_player_index, _first_person_weapon_state_idle, 1u);
        fp_weapon->interpolation_frame_count = 0;
        if ( visible )
            first_person_weapon_set_visibility(local_player_index, 1u);
    }

predict:
    first_person_weapon_predict(local_player_index);
}
