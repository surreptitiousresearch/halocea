/* ai_scripting_conversation_stop @0x83772788 — script bridge (thunk) to ai_conversation_stop. */

#include <stdint.h>

extern void ai_conversation_stop(int16_t conversation_definition_index);

void ai_scripting_conversation_stop(int16_t conversation_index)
{
    ai_conversation_stop(conversation_index);
}
