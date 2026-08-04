/* ai_scripting_conversation_advance @0x83772790 — script bridge (thunk) to ai_conversation_advance. */

#include <stdint.h>

extern void ai_conversation_advance(int16_t conversation_definition_index);

void ai_scripting_conversation_advance(int16_t conversation_index)
{
    ai_conversation_advance(conversation_index);
}
