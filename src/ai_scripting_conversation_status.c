/* ai_scripting_conversation_status @0x837727A0 — script bridge (thunk) to ai_conversation_status. */

#include <stdint.h>

extern int ai_conversation_status(int16_t conversation_definition_index);

int ai_scripting_conversation_status(__int16 conversation_index)
{
    return ai_conversation_status(conversation_index);
}
