/* weapon_play_first_person_weapon_sound @0x8369E060 — fallback path used when a weapon message has no live
 * first-person viewmodel to drive (see first_person_weapon_message_from_weapon): play the first-person
 * sound that the weapon's animation graph associates with the message's animation, as an impulse sound on
 * the weapon object.
 *
 * Resolution chain: message -> first-person weapon state -> first-person animation type -> animation index
 * (via the graph's per-mode animation table) -> animation block -> its sound definition. The sound is
 * flagged for local-player playback when the weapon's owning player (object data +192) is a local player.
 *
 * DEVIATION: the decompiler reported the impulse-sound scale/is_local_player args from the wrong registers
 * (0x82000000 etc.); the disassembly shows scale = 1.0 and is_local_player = the computed local-player flag. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/weapon_definition.h"
#include "headers/player_datum.h"
#include "headers/animation_graph.h"
#include "headers/animation_graph_first_person_weapon_animations.h"
#include "headers/animation.h"
#include "headers/animation_graph_sound_reference.h"
#include "headers/global_tag_instances.h"
#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
#include "headers/object_type.h"
#include "headers/blam_data_globals.h"


#include "headers/real_point3d.h"
#include "headers/real_vector3d.h"
extern void *object_try_and_get_and_verify_type(int object_index, unsigned int valid_type_flags);
extern int16_t first_person_weapon_state_from_weapon_message(int16_t message_type);
extern int16_t first_person_animation_type_from_weapon_state(int16_t state);
extern void *datum_try_and_get(const data_array *data, int index);
extern int object_impulse_sound_new(int object_index, int definition_index, int16_t node_index, const real_point3d *position, const real_vector3d *forward, float scale, uint8_t is_local_player);

void weapon_play_first_person_weapon_sound(int weapon_index, int16_t message_type)
{
    if ( weapon_index == -1 || message_type == -1 )
        return;
    if ( !object_try_and_get_and_verify_type(weapon_index, object_mask_weapon) )
        return;

    object_datum *weapon_data =
        DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, weapon_index)->datum;
    weapon_definition *weapon_def =
        TAG_GET(weapon_definition, weapon_data->definition_index);
    if ( weapon_def->weapon.interface_definition.first_person_animations.index == -1 )
        return;

    int16_t state = first_person_weapon_state_from_weapon_message(message_type);
    if ( state == -1 )
        return;
    int16_t animation_type = first_person_animation_type_from_weapon_state(state);
    if ( animation_type == -1 )
        return;

    animation_graph *graph = TAG_GET(animation_graph, weapon_def->weapon.interface_definition.first_person_animations.index);
    animation_graph_first_person_weapon_animations *animation_mode = 0;
    if ( graph->first_person_weapon_animations.count )
        animation_mode = (animation_graph_first_person_weapon_animations *)
            graph->first_person_weapon_animations.address;

    int16_t animation_index;
    if ( animation_type < 0 || !animation_mode
      || animation_type >= animation_mode->animations.count )
        animation_index = -1;
    else
        animation_index = ((int16_t *)animation_mode->animations.address)[animation_type];
    if ( animation_index == -1 )
        return;

    int16_t graph_animation =
        ((animation *)graph->animations.address)[animation_index].sound_index;
    if ( graph_animation == -1 )
        return;

    int sound_index = ((animation_graph_sound_reference *)graph->sound_references.address)
                          [graph_animation].sound.index;
    if ( sound_index == -1 )
        return;

    uint8_t is_local_player = 0;
    int player_index = weapon_data->object.owner_player_index;
    if ( player_index != -1 )
    {
        player_datum *player = datum_try_and_get(player_data, player_index);
        if ( player && player->local_player_index != -1 )
            is_local_player = 1;
    }

    object_impulse_sound_new(weapon_index, sound_index, -1, global_origin3d, global_forward3d, 1.0f,
                             is_local_player);
}
