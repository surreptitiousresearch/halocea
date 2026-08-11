/* ai_conversation @0x837CFD18 — script entry point to run a conversation by definition index. Validates the
 * index against the scenario, allocates a runtime conversation (ai_conversation_new), and attempts to begin
 * it. Returns 1 if the conversation began or asked to keep trying; otherwise finishes it as "unable to
 * begin" and returns 0. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/blam_data_globals.h"

extern int ai_conversation_new(int16_t conversation_definition_index, char scripted);
extern uint8_t ai_conversation_begin(int conversation_index, uint8_t *continue_trying);
extern void ai_conversation_finish(int conversation_index, uint8_t unable_to_begin, uint8_t success);

int ai_conversation(int16_t conversation_definition_index, uint8_t scripted)
{
    if ( conversation_definition_index >= 0
      && conversation_definition_index < global_scenario->ai_conversations.count )
    {
        int conversation_index = ai_conversation_new(conversation_definition_index, scripted);
        if ( conversation_index != -1 )
        {
            uint8_t continue_trying = 0;
            if ( ai_conversation_begin(conversation_index, &continue_trying) || continue_trying )
                return 1;
            ai_conversation_finish(conversation_index, 1u, 0);
        }
    }
    return 0;
}
