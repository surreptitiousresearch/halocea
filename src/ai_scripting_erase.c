/* ai_scripting_erase @0x8376F870 — erase the actors addressed by an AI index. The low 16 bits are the
 * encounter index; the top two bits select the scope (1 = platoon, 2 = squad), with the platoon/squad
 * index carried in byte 1. */

#include <stdint.h>

extern void ai_erase(int encounter_index, int platoon_index, int squad_index, uint8_t immediate);

void ai_scripting_erase(unsigned int ai_index)
{
    if ( ai_index == -1 )
        return;

    int squad_index = (ai_index >> 8) & 0xFF;
    if ( ai_index >> 30 != 2 )
        squad_index = -1;
    int platoon_index = (ai_index >> 8) & 0xFF;
    if ( ai_index >> 30 != 1 )
        platoon_index = -1;
    ai_erase((uint16_t)ai_index, platoon_index, squad_index, 0);
}
