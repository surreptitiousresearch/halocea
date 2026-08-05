/* ai_scripting_place @0x8376F7B8 — spawn (create) the actors of the addressed encounter scope. The low 16
 * bits are the encounter index; the top two bits select the scope (1 = platoon, 2 = squad), with the
 * platoon/squad index carried in byte 1. */

#include <stdint.h>

extern void encounter_create(int encounter_index, int16_t desired_platoon_index, int16_t desired_squad_index);

void ai_scripting_place(unsigned int ai_index)
{
    if ( ai_index == -1 )
        return;

    int16_t squad_index = (ai_index >> 8) & 0xFF;
    if ( ai_index >> 30 != 2 )
        squad_index = -1;
    int16_t platoon_index = (ai_index >> 8) & 0xFF;
    if ( ai_index >> 30 != 1 )
        platoon_index = -1;
    encounter_create((uint16_t)ai_index, platoon_index, squad_index);
}
