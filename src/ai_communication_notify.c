/* ai_communication_notify @0x837CF360 — broadcast a spoken communication to nearby allied actors so they can
 * react (acknowledge / look). If the packet carries an allegiance-change, applies it first. Then, when the
 * packet conveys information or a look, finds the speaker's head position and BSP location (its ultimate parent
 * if it is mounted) and iterates active actors: any non-self, non-enemy actor within 30 world units that has a
 * base prop for the speaker and can audibly hear it (actor_audibility_at_point >= 2) handles the communication
 * and any accompanying look.
 *
 * Deviations: the actor's head position is compared for the audible-range test (input.position.head_position). The
 * decompiler's final actor_audibility_at_point arg (line_of_sight) is an uninitialised local (v15); passed 0.
 * The packet information_data is the allegiance variant here; its fields are read from the raw union bytes. */

#include <stdint.h>
#include "headers/unit_datum.h"
#include "headers/object_datum.h"
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/actor_iterator.h"
#include "headers/actor_datum.h"
#include "headers/actor_position_data.h"
#include "headers/dialogue_usage.h"
#include "headers/unit_speech_item.h"   /* ai_information_packet */
#include "headers/location.h"
#include "headers/real_point3d.h"
#include "headers/ai_sound_volume.h"
#include "headers/ai_communication_priority.h"
#include "headers/ai_information_type.h"
#include "headers/blam_data_globals.h"

extern const dialogue_usage global_dialogue_table[];

extern void ai_handle_allegiance_broken_notification(int16_t team1_index, int16_t team2_index, uint8_t broken);
extern void unit_get_head_position(int unit_index, real_point3d *head_position);
extern int object_get_ultimate_parent(int object_index);
extern void actor_iterator_new(actor_iterator *iterator, uint8_t active_only);
extern actor_datum *actor_iterator_next(actor_iterator *iterator);
extern uint8_t game_team_is_enemy(int16_t our_team, int16_t other_team);
extern int prop_get_base_by_unit_index(int actor_index, int unit_index, uint8_t acquire, uint8_t refresh_upon_acquisition);
extern void actor_perception_find_sense_position(uint16_t actor_index, const real_point3d *position, int prop_index, actor_position_data *sense_position);
extern int16_t actor_audibility_at_point(uint16_t actor_index, actor_position_data *sense_position, const real_point3d *position, const location *location, int16_t sound_volume, float perception_factor, int16_t line_of_sight);
extern void actor_handle_communication(int actor_index, int prop_index, ai_information_packet *ai_information);
extern void ai_communication_handle_received_looking(int actor_index, uint16_t prop_index, ai_information_packet *information);

void ai_communication_notify(int unit_index, int16_t priority, int16_t vocalization_type,
                             ai_information_packet *ai_information)
{
    if ( ai_information->information_type == _ai_information_allegiance )
        ai_handle_allegiance_broken_notification(
            ai_information->information_data.___u0.allegiance.team1_index,
            ai_information->information_data.___u0.allegiance.team2_index,
            ai_information->information_data.___u0.allegiance.broken);

    if ( !ai_information->information_type && ai_information->look_priority <= 0 )
        return;

    __int16 sound_volume = _ai_sound_volume_medium;
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);
    const location *sound_location = &unit->object.location;   /* unit+152 = _object_datum.location */
    __int16 speaker_team = unit->object.owner_team_index;

    real_point3d speaker_head;
    unit_get_head_position(unit_index, &speaker_head);

    int dialogue_type_index = ai_information->dialogue_type_index;
    if ( dialogue_type_index != -1 && global_dialogue_table[dialogue_type_index].communication_priority >= _ai_communication_priority_communicate )
        sound_volume = _ai_sound_volume_shout;

    if ( unit->object.parent_object_index != -1 )
    {
        int parent = object_get_ultimate_parent(unit_index);
        object_datum *parent_object = (DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, parent)->datum);
        sound_location = &parent_object->object.location;   /* parent+152 = _object_datum.location */
    }

    actor_iterator actors;
    actor_iterator_new(&actors, 1u);
    for ( actor_datum *actor = actor_iterator_next(&actors); actor; actor = actor_iterator_next(&actors) )
    {
        /* recovered: DB decompile reads i->input.position.head_position (actor+0x120); the prior .c
         * used raw offset 0x138 (body_facing), a transcription error — corrected to match the binary */
        const real_point3d *actor_head = &actor->input.position.head_position;
        float dx = speaker_head.n[0] - actor_head->n[0];
        float dy = speaker_head.n[1] - actor_head->n[1];
        float dz = speaker_head.n[2] - actor_head->n[2];
        if ( actor->meta.unit_index == unit_index
          || game_team_is_enemy(actor->meta.team_index, speaker_team)
          || dy * dy + (dx * dx + dz * dz) > 900.0f )
            continue;

        int prop_index = prop_get_base_by_unit_index(actors.index, unit_index, 1u, 1u);
        if ( prop_index == -1 )
            continue;

        actor_position_data sense_position;
        actor_perception_find_sense_position(actors.index, &speaker_head, prop_index, &sense_position);
        if ( actor_audibility_at_point(actors.index, &sense_position, &speaker_head, sound_location,
                                       sound_volume, 1.0f, 0) >= _ai_sound_volume_loud )
        {
            actor_handle_communication(actors.index, prop_index, ai_information);
            ai_communication_handle_received_looking(actors.index, prop_index, ai_information);
        }
    }
}
