/* ai_scripting_place @0x8376F7B8 — spawn (create) the actors of the addressed encounter scope. The low 16
 * bits are the encounter index; the top two bits select the scope (1 = platoon, 2 = squad), with the
 * platoon/squad index carried in byte 1.
 *
 * DEVIATION: the sub-index unpack was `(ai_index >> 8) & 0xFF` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x8376F7C4 / 0x8376F7D8 are `extrwi rX, r3, 8, 8`
 * (rlwinm SH=16 MB=24 ME=31, words 0x546A863E / 0x546B863E), i.e. (x >> 16) & 0xFF.
 * Now AI_INDEX_SUB_INDEX. */

#include <stdint.h>
#include "headers/ai_index.h"

extern void encounter_create(int encounter_index, int16_t desired_platoon_index, int16_t desired_squad_index);

void ai_scripting_place(unsigned int ai_index)
{
    if ( ai_index == -1 )
        return;

    int16_t squad_index = AI_INDEX_SUB_INDEX(ai_index);
    if ( AI_INDEX_SCOPE(ai_index) != _ai_index_squad )
        squad_index = -1;
    int16_t platoon_index = AI_INDEX_SUB_INDEX(ai_index);
    if ( AI_INDEX_SCOPE(ai_index) != _ai_index_platoon )
        platoon_index = -1;
    encounter_create((uint16_t)ai_index, platoon_index, squad_index);
}
