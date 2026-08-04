/* ai_scripting_kill @0x8376F860 — kill every actor addressed by an AI index (with the normal death
 * reaction). */

#include <stdint.h>

extern void ai_scripting_kill_internal(int ai_index, uint8_t silent);

void ai_scripting_kill(int ai_index)
{
    ai_scripting_kill_internal(ai_index, 0);
}
