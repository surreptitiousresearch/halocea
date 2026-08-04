/* ai_scripting_set_blind @0x8376F9D8 — set or clear an encounter's blind flag. */

#include <stdint.h>

extern void encounter_set_blind(uint16_t encounter_index, uint8_t blind);

void ai_scripting_set_blind(int ai_index, uint8_t blind)
{
    if ( ai_index != -1 )
        encounter_set_blind((unsigned __int16)ai_index, blind);
}
