/* ai_communication_initialize_for_new_map @0x837CBBA4 — reset the AI dialogue/communication state for a new
 * map: enable dialogue triggers, clear the per-team chatter/talk/shout timers, reset every dialogue and reply
 * event's throttle, clear the recent-conversation ring, and revalidate the conversation datum array. */

#include <stdint.h>
#include "headers/ai_globals.h"
#include "headers/dialogue_event_status.h"
#include "headers/data_array.h"
#include "headers/blam_data_globals.h"
extern void *memset(void *dst, int value, unsigned int n);


extern void data_make_valid(data_array *data);

void ai_communication_initialize_for_new_map(void)
{
    ai_globals->dialogue_triggers_enabled = 1;
    ai_globals->last_chatter_time[0] = 0;
    ai_globals->last_chatter_time[1] = 0;
    ai_globals->last_talk_time[0] = 0;
    ai_globals->last_talk_time[1] = 0;
    ai_globals->last_shout_time[0] = 0;
    ai_globals->last_shout_time[1] = 0;

    for ( int16_t i = 0; i < 2 * global_dialogue_event_count; i = (int16_t)(i + 1) )
    {
        global_dialogue_events[i].disable_until_time = -1;
        global_dialogue_events[i].last_time_spoken = -1;
    }
    for ( int16_t i = 0; i < 2 * global_reply_event_count; i = (int16_t)(i + 1) )
    {
        global_reply_events[i].disable_until_time = -1;
        global_reply_events[i].last_time_spoken = -1;
    }

    ai_globals->recent_conversation_count = 0;
    ai_globals->recent_conversation_next_index = 0;
    memset(ai_globals->recent_conversations, 0, sizeof(ai_globals->recent_conversations));
    data_make_valid(conversation_data);
}
