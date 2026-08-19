/* ai_scripting_erase @0x8376F870 — erase the actors addressed by an AI index. The low 16 bits are the
 * encounter index; the top two bits select the scope (1 = platoon, 2 = squad), with the platoon/squad
 * index carried in byte 1.
 *
 * DEVIATION: the sub-index unpack was `(ai_index >> 8) & 0xFF` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x8376F87C / 0x8376F890 are `extrwi rX, r3, 8, 8`
 * (rlwinm SH=16 MB=24 ME=31, words 0x5465863E / 0x5464863E), i.e. (x >> 16) & 0xFF.
 * Now AI_INDEX_SUB_INDEX. */

#include <stdint.h>
#include "headers/ai_index.h"

extern void ai_erase(int encounter_index, int platoon_index, int squad_index, uint8_t immediate);

void ai_scripting_erase(unsigned int ai_index)
{
    if ( ai_index == -1 )
        return;

    int squad_index = AI_INDEX_SUB_INDEX(ai_index);
    if ( AI_INDEX_SCOPE(ai_index) != _ai_index_squad )
        squad_index = -1;
    int platoon_index = AI_INDEX_SUB_INDEX(ai_index);
    if ( AI_INDEX_SCOPE(ai_index) != _ai_index_platoon )
        platoon_index = -1;
    ai_erase((uint16_t)ai_index, platoon_index, squad_index, 0);
}
