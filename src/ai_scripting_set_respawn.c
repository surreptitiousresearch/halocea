/* ai_scripting_set_respawn @0x8376F9A8 — set or clear an encounter's respawn flag. */

#include <stdint.h>

extern void encounter_set_respawn(int encounter_index, uint8_t respawn);

void ai_scripting_set_respawn(int ai_index, uint8_t respawn)
{
    if ( ai_index != -1 )
        encounter_set_respawn((unsigned __int16)ai_index, respawn);
}
