/* unit_make_damage_sound @0x837BADD0 — pick and queue the unit's AI vocalization for a damage event (a pain
 * grunt, a death cry, a "no shields" warning, etc.), throttled by per-unit speech cooldown counters. Returns
 * 1 if a vocalization was actually spoken. Units with no dialogue tag (unit.dialogue_index == -1) are silent.
 *
 * Selection: on death, the cry depends on the damage vocalization category (from the damage effect [227]),
 * whether the kill was "instant"/explosive, and how reckless/large the hit was; otherwise a pain sound is
 * chosen subject to the pain cooldown and a random gate. The chosen vocalization is run through
 * unit_test_speech, and on success unit_speak queues it; a matching cooldown is then armed and, for the
 * death path, an AI unit-effect is raised.
 *
 * ABI matches the call site (0x836D5E24): body_damage/shield_damage are single floats; shield_damage is
 * unused by the body. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/global_tag_instances.h"
#include "headers/damage_effect_definition.h"
#include "headers/damage_data.h"
#include "headers/unit_speech_item.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/ai_vocalization_type.h"
#include "headers/damage_category.h"
#include "headers/ai_unit_effect.h"
#include "headers/unit_speech_priority.h"
#include "headers/blam_data_globals.h"

#include "headers/ai_information_packet.h"
extern uint32_t *get_global_random_seed_address(void);
extern float real_seed_random(uint32_t *seed);
extern int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup, uint8_t allow_queue, int *unit_last_speech_time, int16_t *vocalization_type_reference, int *sound_definition_index_reference);
extern void ai_communication_packet_new(ai_information_packet *information);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);
extern void ai_handle_unit_effect(int owner_unit_index, int16_t effect_type, int16_t volume);

int unit_make_damage_sound(int unit_index, damage_data *damage_data, uint8_t died,
                           uint8_t died_instantly, float body_damage, float shield_damage)
{
    uint8_t spoke = 0;
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum,
                                                        unit_index)->datum;
    if ( unit->unit.dialogue_index == -1 )
        return spoke;

    float body_damage_taken = unit->object.recent_body_damage;
    __int16 vocalization = -1;
    __int16 damage_vocalization_category = 0;
    __int16 chosen_vocalization = -1;        /* byref into unit_test_speech */
    unsigned __int8 took_body_damage = body_damage_taken > 0.0f;
    unsigned __int8 heavy_damage = body_damage_taken >= 0.60000002f;
    char is_pain_or_death = 0;
    __int16 effect_type = -1;
    __int16 effect_param = 0;
    int died_flag = died;

    if ( damage_data && damage_data->definition_index != -1 )
        damage_vocalization_category = TAG_GET(damage_effect_definition, damage_data->definition_index)->damage.category;   /* +454 */

    if ( died )
    {
        int actor_index = unit->unit.swarm_actor_index;
        if ( actor_index == -1 )
            actor_index = unit->unit.actor_index;

        unsigned __int8 severe = 0;
        unsigned __int8 instant_kill = 0;
        if ( damage_data->definition_index != -1 )
            instant_kill = TAG_GET(damage_effect_definition, damage_data->definition_index)->damage.instantaneous_acceleration >= 2.0f;   /* +500 */

        if ( actor_index == -1 )
            severe = body_damage_taken > body_damage + 0.2f;
        else
            severe = *((__int16 *)actor_data->data + 914 * (unsigned __int16)actor_index + 55) >= 3;

        if ( damage_vocalization_category == _damage_category_falling )
            vocalization = _vocalization_death_falling;
        else if ( damage_vocalization_category == _damage_category_flame )
            vocalization = _vocalization_death_agonizing;
        else if ( instant_kill )
            vocalization = _vocalization_death_flying;
        else if ( severe )
            vocalization = died_instantly == 0 ? _vocalization_death_violent : _vocalization_death_instant;
        else
            vocalization = _vocalization_death_quiet;

        chosen_vocalization = vocalization;
        is_pain_or_death = 1;
        if ( vocalization != _vocalization_death_quiet )
        {
            effect_type = _ai_unit_effect_death_scream;
            effect_param = vocalization != _vocalization_death_instant ? 1 : 4;
        }
    }
    else if ( !unit->unit.speech.damage_major_timer )  /* not in pain cooldown */
    {
        if ( damage_vocalization_category == _damage_category_falling )
        {
            vocalization = _vocalization_pain_falling;
            is_pain_or_death = 1;
            chosen_vocalization = vocalization;
        }
        else if ( body_damage_taken >= 0.60000002f )
        {
            vocalization = _vocalization_pain_body_major;
            is_pain_or_death = 1;
            chosen_vocalization = vocalization;
        }
        else if ( !unit->unit.speech.damage_minor_timer && unit->unit.speech.damage_minor_sounds < 3 )
        {
            if ( !unit->unit.speech.current.priority
              || real_seed_random(get_global_random_seed_address()) < 0.40000001f )
            {
                vocalization = !took_body_damage ? _vocalization_pain_shield : _vocalization_pain_body;
                chosen_vocalization = vocalization;
            }
        }
    }

    if ( vocalization != -1 )
    {
        int sound_definition_index = -1;
        __int16 speech_category = died_flag ? _unit_speech_death
                                            : (is_pain_or_death == 0 ? _unit_speech_pain : _unit_speech_involuntary);
        __int16 speech_handle = unit_test_speech(unit_index, speech_category, 1u, 0, 0,
                                                 &chosen_vocalization, &sound_definition_index);
        if ( speech_handle > 0 )
        {
            unit_speech_item items[3];
            for ( int i = 0; i < (int)sizeof(items); ++i )
                ((char *)items)[i] = 0;
            items[0].priority = speech_category;
            items[0].pause_time = 7;
            items[0].vocalization_type = chosen_vocalization;
            items[0].sound_definition_index = sound_definition_index;
            ai_communication_packet_new(&items[0].ai);
            unit_speak(unit_index, speech_handle, items);
            spoke = 1;
        }
    }

    if ( spoke )
    {
        if ( heavy_damage )
        {
            unit->unit.speech.damage_major_timer = 60;
        }
        else
        {
            __int16 pain_count = unit->unit.speech.damage_minor_sounds;
            unit->unit.speech.damage_minor_timer = 30;
            unit->unit.speech.damage_minor_decay_timer = 22;
            unit->unit.speech.damage_minor_sounds = pain_count + 1;
        }
    }

    if ( effect_type != -1 )
        ai_handle_unit_effect(unit_index, effect_type, effect_param);

    return spoke;
}
