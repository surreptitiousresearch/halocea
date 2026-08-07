/* unit_notify_impulse_sound @0x837BACD0 — announce an impulse (non-dialogue) sound the unit just played:
 * requests a low-priority speech slot (priority 6, or the unit's own higher-priority pending speech if
 * greater), queues a zeroed unit_speech_item for it (24-tick pause), starts the AI communication event via
 * the unit's own ai_information_packet (unit +920, distinct from the dialogue-vocalization packet at
 * +904 used by unit_dialogue_update.c), stores the impulse sound index (unit +1024) and marks it pending
 * (unit +1012 flag, +1016 delay reset), then forwards to the hcex bridge for engine-side audio playback. */

#include <stdint.h>
#include <string.h>
#include "headers/data_array.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/unit_speech_item.h"
#include "headers/ai_information_packet.h"
#include "headers/unit_speech_priority.h"
#include "headers/blam_data_globals.h"


#include "headers/ai_information_packet.h"
#include "headers/ai_information_packet.h"
extern int16_t unit_test_speech(int unit_index, int16_t priority, uint8_t allow_recursive_lookup, uint8_t allow_queue, int *unit_last_speech_time, int16_t *vocalization_type_reference, int *sound_definition_index_reference);
extern void ai_communication_packet_new(ai_information_packet *information);
extern void unit_speak(int unit_index, int16_t play_type, const unit_speech_item *speech_item);
extern void ai_communication_started(int unit_index, int16_t priority, int16_t vocalization_type, const ai_information_packet *ai_information);
extern void hcex_on_play_sound(int unit_index, int sound_definition_index, int impulse_sound_index);

void unit_notify_impulse_sound(int unit_index, int sound_definition_index, int impulse_sound_index)
{
    unit_datum *unit = (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;

    int16_t vocalization_type = -1;
    int test_sound_definition_index = sound_definition_index;
    int16_t priority = unit_test_speech(unit_index, _unit_speech_scripted, 0, 0, 0, &vocalization_type, &test_sound_definition_index);
    if ( priority <= _unit_speech_pain )
        priority = _unit_speech_pain;

    unit_speech_item item;
    memset(&item, 0, sizeof(item));
    item.sound_definition_index = sound_definition_index;
    item.priority = _unit_speech_scripted;
    item.vocalization_type = -1;
    item.pause_time = 24;
    ai_communication_packet_new(&item.ai);
    unit_speak(unit_index, priority, &item);

    unit->unit.speech.impulse_sound_index = impulse_sound_index;
    unit->unit.speech.played = 1;
    unit->unit.speech.pre_delay_timer = 0;

    ai_communication_started(unit_index, _unit_speech_scripted, -1, &unit->unit.speech.current.ai);
    hcex_on_play_sound(unit_index, sound_definition_index, impulse_sound_index);
}
