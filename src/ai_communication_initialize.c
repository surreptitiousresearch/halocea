/* ai_communication_initialize @ 0x837CBA18 — count dialogue/reply table entries,
 * allocate per-event status arrays, build the communication-type index map, and
 * create the conversation pool. */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/dialogue_usage.h"
#include "headers/reply_usage.h"
#include "headers/dialogue_event_status.h"
#include "headers/blam_data_globals.h"

extern const dialogue_usage *global_dialogue_table;
extern const reply_usage *global_reply_table;

extern void *game_state_malloc(const char *name, const char *type, int size);
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);

void ai_communication_initialize(void)
{
    const dialogue_usage *d;
    const reply_usage *r;
    int16_t dialogue_count;
    int16_t reply_count;
    int i;

    dialogue_count = 0;
    d = global_dialogue_table;
    do { ++d; ++dialogue_count; } while ( d->communication_type != -1 );
    global_dialogue_event_count = dialogue_count;
    if ( !global_dialogue_events )
        global_dialogue_events = game_state_malloc(
            "ai communication dialogue", 0, 16 * dialogue_count);

    reply_count = 0;
    r = global_reply_table;
    do { ++r; ++reply_count; } while ( r->original_vocalization_type != -1 );
    global_reply_event_count = reply_count;
    if ( !global_reply_events )
        global_reply_events = game_state_malloc(
            "ai communication replies", 0, 16 * reply_count);

    for ( i = 0; i < 57; ++i )
    {
        const dialogue_usage *scan = global_dialogue_table;
        int16_t index = 0;
        int communication_type = 0;
        global_communication_table_indices[i] = -1;
        while ( communication_type != i )
        {
            ++scan;
            communication_type = scan->communication_type;
            ++index;
            if ( communication_type == -1 )
                goto next;
        }
        global_communication_table_indices[i] = index;
    next:;
    }

    conversation_data = game_state_data_new("ai conversation", 8, 100);
}
