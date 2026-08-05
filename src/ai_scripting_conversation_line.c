/* ai_scripting_conversation_line @0x83772798 — script bridge (thunk) to ai_conversation_line. */

#include <stdint.h>

extern int ai_conversation_line(int16_t conversation_definition_index);

int ai_scripting_conversation_line(int16_t conversation_index)
{
    return ai_conversation_line(conversation_index);
}
