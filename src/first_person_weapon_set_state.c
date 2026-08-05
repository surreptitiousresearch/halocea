/* first_person_weapon_set_state @0x8369F950 — drive a local player's first-person viewmodel into `new_state`,
 * subject to transition gating, and start the corresponding first-person animation.
 *
 * Steps:
 *   1. Remap a couple of states for "attached" weapons (weapon flags bit 0): ready->overheating_again,
 *      throw_grenade->throw_overheated.
 *   2. Gate the transition by the current state: fire/misfire states are rejected unless the viewmodel is
 *      idle or in an allowed prior state; light on/off require idle-or-posing; re-entering ready is
 *      rejected. A rejected transition sets the target to -1 and does nothing.
 *   3. Shotgun special-case: for a single-round-reload weapon (weapon_type == 3) entering overheated,
 *      collapse to idle unless the weapon's flags bit 0 is set.
 *   4. Resolve the animation type, pick an interpolation blend length, and if the resolved animation exists:
 *      optionally stop the current looping sound (when reset_sounds is set), start the interpolation, and
 *      commit the new state + animation.
 *
 * The first-person animation graph is the weapon definition's first_person_animations reference; the
 * per-type animation index comes from the graph's first first-person-weapon animation set. */

#include <stdint.h>
#include "headers/first_person_weapon.h"
#include "headers/weapon_datum_flags.h"
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_datum.h"
#include "headers/weapon_definition.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_first_person_weapon_animations.h"
#include "headers/first_person_weapon_state.h"
#include "headers/first_person_weapon_animation.h"
#include "headers/weapon_type.h"
#include "headers/blam_data_globals.h"


extern int16_t first_person_animation_type_from_weapon_state(int16_t state);
extern void sound_stop_impulse(uint16_t sound_index);
extern void first_person_weapon_start_interpolation(int16_t local_player_index, int16_t frame_count);

void first_person_weapon_set_state(int16_t local_player_index, int16_t new_state, uint8_t reset_sounds)
{
    first_person_weapon *fp_weapon = &first_person_weapons[local_player_index];
    int16_t target_state = new_state;
    int weapon_index = first_person_weapons[local_player_index].weapon_index;

    if ( weapon_index != -1 )
    {
        weapon_datum *weapon_data = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                            first_person_weapons[local_player_index].weapon_index)->datum;
        if ( (weapon_data->weapon.flags & (1u << _weapon_overheated_bit)) != 0 )   /* attached/reloading weapon: remap a few states */
        {
            if ( new_state == _first_person_weapon_state_ready )
                target_state = _first_person_weapon_state_overheating_again;
            else if ( new_state == _first_person_weapon_state_throw_grenade )
                target_state = _first_person_weapon_state_throw_grenade_overheated;
        }
    }

    /* transition gating against the current viewmodel state */
    int current_state = fp_weapon->state;
    if ( target_state == _first_person_weapon_state_primary_fire || target_state == _first_person_weapon_state_secondary_fire || target_state == _first_person_weapon_state_primary_misfire || target_state == _first_person_weapon_state_secondary_misfire )
    {
        if ( current_state != _first_person_weapon_state_idle && current_state != _first_person_weapon_state_posing && current_state != _first_person_weapon_state_primary_fire && current_state != _first_person_weapon_state_charged
             && current_state != _first_person_weapon_state_shotgun_enter_reload && current_state != _first_person_weapon_state_overheated_exit && current_state != _first_person_weapon_state_shotgun_exit_reload_empty && current_state != _first_person_weapon_state_shotgun_exit_reload_full
             && current_state != _first_person_weapon_state_reload_while_empty && current_state != _first_person_weapon_state_reload_while_full )
            target_state = -1;
    }
    else if ( target_state == _first_person_weapon_state_light_off || target_state == _first_person_weapon_state_light_on )
    {
        if ( current_state != _first_person_weapon_state_idle && current_state != _first_person_weapon_state_posing )
            target_state = -1;
    }
    else if ( target_state == _first_person_weapon_state_ready )
    {
        if ( current_state == _first_person_weapon_state_ready )
            target_state = -1;
    }

    if ( target_state == -1 || weapon_index == -1 )
        return;

    weapon_datum *weapon = (weapon_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *definition = TAG_GET(weapon_definition, weapon->definition_index);
    if ( definition->weapon.weapon_type == _weapon_type_plasma_pistol && target_state == _first_person_weapon_state_overheated )
        target_state = (weapon->weapon.flags & (1u << _weapon_overheated_bit)) == 0 ? _first_person_weapon_state_idle : target_state;

    int16_t animation_type = first_person_animation_type_from_weapon_state(target_state);

    int16_t interpolation_frames;
    if ( animation_type == _first_person_weapon_animation_posing && fp_weapon->state == _first_person_weapon_state_shotgun_exit_reload_empty )
    {
        interpolation_frames = 0;
    }
    else
    {
        switch ( target_state )
        {
            case _first_person_weapon_state_overheated:
            case _first_person_weapon_state_melee:
            case _first_person_weapon_state_ready:
                interpolation_frames = 0;
                break;
            case _first_person_weapon_state_primary_fire:
            case _first_person_weapon_state_secondary_fire:
            case _first_person_weapon_state_primary_misfire:
            case _first_person_weapon_state_secondary_misfire:
                interpolation_frames = 3;
                break;
            default:
                interpolation_frames = 6;
                break;
        }
    }

    if ( fp_weapon->unit_index == -1 )
        return;

    int first_person_animation_graph = definition->weapon.interface_definition.first_person_animations.index;
    animation_graph *graph = TAG_GET(animation_graph, first_person_animation_graph);
    if ( !graph->first_person_weapon_animations.count )
        return;
    animation_graph_first_person_weapon_animations *animation_set =
        (animation_graph_first_person_weapon_animations *)graph->first_person_weapon_animations.address;
    if ( !animation_set )
        return;

    int16_t animation_index;
    if ( animation_type < 0 || animation_type >= animation_set->animations.count )
        animation_index = -1;
    else
        animation_index = ((int16_t *)animation_set->animations.address)[animation_type];
    if ( animation_index == -1 )
        return;

    if ( reset_sounds )
    {
        int current_sound_index = fp_weapon->current_sound_index;
        if ( current_sound_index != -1 && fp_weapon->current_sound_state != 1 )
        {
            sound_stop_impulse(current_sound_index);
            fp_weapon->current_sound_index = -1;
            fp_weapon->current_sound_state = -1;
        }
    }

    if ( interpolation_frames > 0 )
        first_person_weapon_start_interpolation(local_player_index, interpolation_frames);

    fp_weapon->state = target_state;
    fp_weapon->state_animation.index = animation_index;
    fp_weapon->state_animation.frame_index = 0;
}
