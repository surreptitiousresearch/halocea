#include <stdint.h>
/* ai_scripting_conversation @0x83772780 — script bridge: run the conversation with the given definition
 * index, flagged as scripted. */

extern int ai_conversation(int16_t conversation_definition_index, uint8_t scripted);

uint8_t ai_scripting_conversation(int16_t conversation_index)
{
    return ai_conversation(conversation_index, 1u);
}
