/* ai_index_actor_iterator_new @0x8376F1A8 — initialize an iterator over the actors selected by an AI index.
 * The low 16 bits of the AI index are the encounter index; the top two bits select a sub-scope: 1 = restrict
 * to a platoon (platoon index in byte 1), 2 = restrict to a squad (squad index in byte 1), 3 = invalid (no
 * actors). The underlying encounter actor iterator walks every actor in the encounter; ai_index_actor_iterator_next
 * filters by the squad/platoon restriction. An out-of-range or uninitialized state marks the iterator empty
 * (encounter_index = -1).
 *
 * DEVIATION: the sub-index unpack was `(ai_index >> 8) & 0xFF` — Hex-Rays' BYTE1 expanded with the
 * little-endian value form. Disasm 0x8376F218 / 0x8376F224 are `extrwi r10, r3, 8, 8`
 * (rlwinm SH=16 MB=24 ME=31, word 0x546A863E), i.e. (x >> 16) & 0xFF. Now AI_INDEX_SUB_INDEX. */

#include "headers/ai_index_actor_iterator.h"
#include "headers/scenario.h"
#include "headers/ai_globals.h"
#include "headers/ai_index.h"
#include "headers/blam_data_globals.h"

extern void encounter_actor_iterator_new(encounter_actor_iterator *iterator, int encounter_index);

void ai_index_actor_iterator_new(unsigned int ai_index, ai_index_actor_iterator *iterator)
{
    scenario *scen = global_scenario;
    iterator->encounter_index = (unsigned short)ai_index;

    if ( scen && ai_globals->ai_initialized_for_map && (unsigned short)ai_index < scen->ai_encounters.count )
    {
        iterator->platoon_index = -1;
        iterator->squad_index = -1;
        if ( AI_INDEX_SCOPE(ai_index) )
        {
            if ( AI_INDEX_SCOPE(ai_index) == _ai_index_platoon )
            {
                iterator->platoon_index = AI_INDEX_SUB_INDEX(ai_index);
            }
            else if ( AI_INDEX_SCOPE(ai_index) == 3 )   /* reserved/invalid scope (no DB name) */
            {
                iterator->encounter_index = -1;
                return;
            }
            else   /* _ai_index_squad */
            {
                iterator->squad_index = AI_INDEX_SUB_INDEX(ai_index);
            }
        }
        encounter_actor_iterator_new(&iterator->iterator, (unsigned short)ai_index);
    }
    else
    {
        iterator->encounter_index = -1;
    }
}
