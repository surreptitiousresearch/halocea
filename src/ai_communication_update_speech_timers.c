/* ai_communication_update_speech_timers @0x837CC5E0 — after a unit speaks, push back the AI dialogue timers so
 * the same kind of line isn't repeated too soon. Sets the unit's next-allowed-speech time (object +0x3F0 from
 * its post-speech delay at +0x3FA), resets the speaking actor's idle-vocalization timer, then for the actor's
 * communication team raises the per-priority shout/talk/chatter cooldowns (ai_globals last_chatter/last_talk/
 * last_shout_time[team]) and stamps the spoken dialogue/reply table rows' last-spoken and disable-until times.
 *
 * Deviation: the decompiler indexes ai_globals via &ai_active + byte offsets; those resolve exactly to
 * last_chatter_time[team] (+0x14), last_talk_time[team] (+0x1C) and last_shout_time[team] (+0x24), used here. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/actor_datum.h"
#include "headers/object_header_datum.h"
#include "headers/unit_datum.h"
#include "headers/ai_globals.h"
#include "headers/dialogue_usage.h"
#include "headers/dialogue_event_status.h"
#include "headers/ai_communication_priority.h"
#include "headers/blam_data_globals.h"

extern const dialogue_usage   global_dialogue_table[];
extern const dialogue_usage   global_reply_table[];
extern int game_time_get(void);
extern void actor_reset_idle_vocalization_timer(uint16_t actor_index);
extern int16_t actor_communication_team(uint16_t actor_index);

/* Attestation: the binary reads only r3-r6 (four params); callers set up only four
 * arguments. The stale DB prototype's 5th param `reply_table_index` was a phantom and
 * has been dropped. r5=vocalization_type indexes the dialogue table, r6=dialogue_type_index
 * indexes the reply table (confirmed at 0x837CC734 / 0x837CC7AC). */
void ai_communication_update_speech_timers(int unit_index, int16_t priority, int16_t vocalization_type,
                                           int16_t dialogue_type_index)
{
    unit_datum *unit =
        (unit_datum *)DATA_ARRAY_ELEMENT(object_header_data, object_header_datum, unit_index)->datum;
    int actor_index = unit->unit.actor_index;
    actor_datum *actor = actor_index == -1 ? nullptr
                       : DATA_ARRAY_ELEMENT(actor_data, actor_datum, actor_index);

    int now = game_time_get();
    /* +1018 / +1008 in _unit_datum's speech block */
    int post_delay = unit->unit.speech.sound_timer - 45;
    int next_speech_time = (__CFADD__(post_delay, 0x80000000) ? 0 : post_delay) + now;
    unit->unit.speech.last_speech_finished_time = next_speech_time;

    if ( actor )
        actor_reset_idle_vocalization_timer(actor_index);

    int16_t team = actor ? actor_communication_team(actor_index) : -1;
    if ( team == -1 )
        return;

    if ( priority <= _ai_communication_priority_shout )
    {
        if ( ai_globals->last_chatter_time[team] <= next_speech_time )
            ai_globals->last_chatter_time[team] = next_speech_time;
        if ( priority >= _ai_communication_priority_talk && ai_globals->last_talk_time[team] <= next_speech_time )
            ai_globals->last_talk_time[team] = next_speech_time;
        if ( priority >= _ai_communication_priority_shout && ai_globals->last_shout_time[team] <= next_speech_time )
            ai_globals->last_shout_time[team] = next_speech_time;
    }

    if ( vocalization_type != -1 )
    {
        dialogue_event_status *event = &global_dialogue_events[2 * vocalization_type + team];
        event->last_time_spoken = now;
        if ( global_dialogue_table[vocalization_type].repeat_delay > 0.0f )
            event->disable_until_time =
                (int)(global_dialogue_table[vocalization_type].repeat_delay * 30.0f + (float)next_speech_time);
    }

    if ( dialogue_type_index != -1 )
    {
        dialogue_event_status *event = &global_reply_events[2 * dialogue_type_index + team];
        event->last_time_spoken = now;
        if ( global_reply_table[dialogue_type_index].repeat_delay > 0.0f )
            event->disable_until_time =
                (int)(global_reply_table[dialogue_type_index].repeat_delay * 30.0f + (float)next_speech_time);
    }
}
