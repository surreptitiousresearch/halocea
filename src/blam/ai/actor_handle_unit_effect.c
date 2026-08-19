/* actor_handle_unit_effect @0x8371F928 — apply a heard/sensed "unit effect" (a noise an aware-of unit made) to
 * the actor's prop for that unit, latching the strongest recent effect and raising the matching combat
 * stimulus. The effect is only latched if it outranks the prop's current unit_effect (+0x66), and its decay
 * window (+0x68) is set (150 ticks for effect 3, else 30). The per-effect handling is gated on the prop not
 * being ignored, and (for effects 1/2) on the actor's encounter not being deaf (encounter->deaf):
 *   0 = bump         -> stimulus_bumped
 *   1 = shooting     -> shooting flag, wake if player, stimulus_heard_shooting
 *   2 = death_scream -> dead flag, wake, stimulus_prop_just_killed
 *   3 = magic_sight  -> wake if player, perception_become_acknowledged
 * All paths bump the prop's perception (+0x30) and refresh flag (+0x64). */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/encounter_datum.h"
#include "headers/prop_datum.h"
#include "headers/ai_sound_volume.h"
#include "headers/ai_unit_effect.h"
#include "headers/blam_data_globals.h"

extern void actor_set_dormant(int actor_index, uint8_t dormant);
extern void actor_stimulus_bumped(int actor_index, int prop_index);
extern void actor_stimulus_heard_shooting(int actor_index, int prop_index);
extern void actor_stimulus_prop_just_killed(int actor_index, int prop_index);
extern uint8_t actor_perception_become_acknowledged(int actor_index, int prop_index, uint8_t *expected_reference);

void actor_handle_unit_effect(int actor_index, int prop_index, int16_t effect_type)
{
    prop_datum *prop = DATA_ARRAY_ELEMENT(prop_data, prop_datum, prop_index);

    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);
    int encounter_index = actor->meta.encounter_index;
    char encounter_deaf = 0;
    if ( encounter_index != -1 )
        encounter_deaf = DATA_ARRAY_ELEMENT(encounter_data, encounter_datum, encounter_index)->deaf;

    int16_t current_effect = prop->unit_effect;
    unsigned int latched_effect = effect_type;
    if ( current_effect == -1 || current_effect <= effect_type )
    {
        prop->unit_effect = effect_type;
        prop->unit_effect_decay_ticks = (effect_type != _ai_unit_effect_magic_sight) ? 30 : 150;
    }

    if ( latched_effect >= NUMBER_OF_AI_UNIT_EFFECTS )
        return;

    if ( latched_effect == _ai_unit_effect_bump )
    {
        if ( !prop->ignore )
        {
            prop->ineffability = 3;
            prop->perception = 3;
            prop->refresh_stimuli = 1;
            actor_stimulus_bumped(actor_index, prop_index);
        }
    }
    else if ( latched_effect == _ai_unit_effect_shooting )
    {
        if ( !encounter_deaf && !prop->ignore )
        {
            uint8_t was_player = prop->player;
            prop->shooting = 1;
            prop->audibility = _ai_sound_volume_shout;
            prop->perception = 3;
            prop->refresh_stimuli = 1;
            if ( was_player )
                actor_set_dormant(actor_index, 0);
            actor_stimulus_heard_shooting(actor_index, prop_index);
        }
    }
    else if ( latched_effect == _ai_unit_effect_death_scream )
    {
        if ( !encounter_deaf && !prop->ignore )
        {
            prop->dead = 1;
            prop->audibility = _ai_sound_volume_shout;
            prop->perception = 3;
            prop->refresh_stimuli = 1;
            actor_set_dormant(actor_index, 0);
            actor_stimulus_prop_just_killed(actor_index, prop_index);
        }
    }
    else /* latched_effect == _ai_unit_effect_magic_sight */
    {
        if ( !prop->ignore )
        {
            uint8_t was_player = prop->player;
            prop->ineffability = 3;
            prop->perception = 3;
            prop->refresh_stimuli = 1;
            if ( was_player )
                actor_set_dormant(actor_index, 0);
            actor_perception_become_acknowledged(actor_index, prop_index, 0);
        }
    }
}
