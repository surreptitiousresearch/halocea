/* ai_scripting_set_deaf @0x8376F9C0 — set or clear an encounter's deaf flag. */

#include <stdint.h>

extern void encounter_set_deaf(uint16_t encounter_index, uint8_t deaf);

void ai_scripting_set_deaf(int ai_index, uint8_t deaf)
{
    if ( ai_index != -1 )
        encounter_set_deaf((uint16_t)ai_index, deaf);
}
