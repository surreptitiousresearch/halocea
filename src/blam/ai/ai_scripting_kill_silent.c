/* ai_scripting_kill_silent @0x8376F868 — silently kill every actor addressed by an AI index (no death
 * reaction or dialogue). */

#include <stdint.h>

extern void ai_scripting_kill_internal(int ai_index, uint8_t silent);

void ai_scripting_kill_silent(int ai_index)
{
    ai_scripting_kill_internal(ai_index, 1u);
}
