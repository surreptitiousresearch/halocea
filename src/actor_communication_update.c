/* actor_communication_update @ 0x837CBE10 — drive idle/combat AI vocalizations. Only runs for actors at or
 * above action category 2 (combat) while dialogue triggers are enabled. Resets the idle-vocalization timer
 * whenever it is unset or the combat state just changed, then counts the timer down; when it reaches zero it
 * asks unit_test_speech for an appropriate line (seeded with the current combat state as the vocalization
 * type) and, if one is selected, builds a speech item and speaks it through the actor's unit.
 *
 * The decompiler expanded the speech-item zero-init into two store loops; reconstructed as a single
 * memset(0) over the struct (the per-field clears it produces are subsumed by that). */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/ai_globals.h"
#include "headers/unit_speech_item.h"
#include "headers/actor_mode.h"
#include "headers/unit_speech_priority.h"
#include "headers/ai_vocalization_type.h"
#include "headers/blam_data_globals.h"


extern uint8_t actor_in_combat(uint16_t actor_index);
extern void actor_reset_idle_vocalization_timer(uint16_t actor_index);
extern int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup, uint8_t allow_queue, int *unit_last_speech_time, int16_t *vocalization_type_reference, int *sound_definition_index_reference);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);

void actor_communication_update(int actor_index)
{
    actor_datum *actor = DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    if ( actor->state.mode < _actor_mode_alert || !ai_globals->dialogue_triggers_enabled )
        return;

    uint8_t in_combat = actor_in_combat(actor_index);

    if ( !actor->control.idle_vocalization_timer || actor->control.idle_vocalization_combat != in_combat )
        actor_reset_idle_vocalization_timer(actor_index);

    int16_t timer = actor->control.idle_vocalization_timer;
    if ( timer <= 0 )
        return;

    timer = timer - 1;
    actor->control.idle_vocalization_timer = timer;
    if ( timer )
        return;

    int     unit_index = actor->meta.unit_index;              /* meta.unit_index */
    int     sound_definition_index = -1;
    int16_t vocalization_type = in_combat ? _vocalization_idle_combat : _vocalization_idle_noncombat;
    int16_t play_type = unit_test_speech(unit_index, _unit_speech_idle, 1u, 0, NULL,
                                         &vocalization_type, &sound_definition_index);
    if ( play_type > 0 )
    {
        unit_speech_item speech_item;
        memset(&speech_item, 0, sizeof(speech_item));
        speech_item.priority = _unit_speech_idle;
        speech_item.vocalization_type = vocalization_type;
        speech_item.sound_definition_index = sound_definition_index;
        memset(&speech_item.ai, 255, 10);
        unit_speak(unit_index, play_type, &speech_item);
    }
}
