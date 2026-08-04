/* unit_scream @ 0x837BB1A0 — make a unit emit a scream/vocalization of the given type. Maps scream_type
 * to a vocalization index (0=pain 10, 1=death random[39|11], 2=11, 3=12, 4=13, other=183), looks up the
 * matching sound permutation in the unit's dialogue tag (unit.dialogue_index; entry stride 16 words... 4 words, the
 * sound_definition_index lives at word [4*vocalization+7]), tests whether the unit may speak it, and if so
 * queues a unit_speech_item via unit_speak. Returns 1 if the unit spoke, 0 otherwise. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/global_tag_instances.h"
#include "headers/unit_speech_item.h"
#include "headers/ai_vocalization_type.h"
#include "headers/unit_scream_type.h"
#include "headers/unit_speech_priority.h"
#include "headers/blam_data_globals.h"
#include "headers/ai_information_packet.h"
extern void *memset(void *dst, int value, unsigned int n);


extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup, uint8_t allow_queue, int *unit_last_speech_time, int16_t *vocalization_type_reference, int *sound_definition_index_reference);
extern void ai_communication_packet_new(ai_information_packet *information);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);

int unit_scream(int unit_index, int16_t scream_type)
{
    unit_datum *unit = ((unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum);

    __int16 vocalization;
    /* The dialogue-tag vocalization index is an ai_vocalization_type (same domain unit_make_damage_sound
     * feeds to unit_speak). scream_type is a unit_scream_type (DB enum _67894A93...; callers pass
     * _unit_scream_falling / _unit_scream_resurrection etc.). */
    switch ( scream_type )
    {
        case _unit_scream_falling:  vocalization = _vocalization_scream_fear; break;
        case _unit_scream_grenade_attached_to_us:
            if ( real_seed_random(get_global_random_seed_address()) >= 0.5 )
                vocalization = _vocalization_scream_pain;
            else
                vocalization = _vocalization_hurt_enemy_grenade;
            break;
        case _unit_scream_burning_to_death:  vocalization = _vocalization_scream_pain; break;
        case _unit_scream_destroyed_limb:    vocalization = _vocalization_maimed_limb; break;
        case _unit_scream_destroyed_head:    vocalization = _vocalization_maimed_head; break;
        default: /* _unit_scream_resurrection */ vocalization = _vocalization_resurrect; break;
    }

    int dialogue_tag = unit->unit.dialogue_index;
    if ( dialogue_tag != -1 )
    {
        __int16 vocalization_arg = vocalization;
        int sound_definition_index = TAG_GET(int, dialogue_tag)[4 * vocalization + 7];
        if ( sound_definition_index != -1 )
        {
            __int16 speech_handle = unit_test_speech(unit_index, _unit_speech_scream, 1u, 0, nullptr,
                &vocalization_arg, &sound_definition_index);
            if ( speech_handle > 0 )
            {
                unit_speech_item item;
                memset(&item, 0, sizeof(item));
                item.priority = _unit_speech_scream;
                item.pause_time = 7;
                item.vocalization_type = vocalization_arg;
                item.sound_definition_index = sound_definition_index;
                ai_communication_packet_new(&item.ai);
                unit_speak(unit_index, speech_handle, &item);
                return 1;
            }
        }
    }
    return 0;
}
