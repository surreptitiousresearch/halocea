/* game_allegiance_initialize_for_new_map @0x837452A8 — reset the AI team-alliance tables for a new map: no
 * allegiances, cleared ally/friendly bitvectors, then mark every team as allied/friendly with itself.
 *
 * Deviation: the decompiler indexes the self-allegiance bits with raw pointer arithmetic
 * (&allegiance_count + 4*((11*i>>5)+41)); reproduced verbatim — it treats friendly_bitvector as a flat bit
 * array, setting bit (11*team) for each of the 10 teams. */

#include <stdint.h>
#include "headers/game_allegiance.h"

void game_allegiance_initialize_for_new_map(void)
{
    struct_game_allegiance_globals *globals = game_allegiance_globals;
    globals->allegiance_count = 0;
    globals->ally_bitvector[0] = 0;
    globals->ally_bitvector[1] = 0;
    globals->ally_bitvector[2] = 0;
    globals->ally_bitvector[3] = 0;
    globals->friendly_bitvector[0] = 0;
    globals->friendly_bitvector[1] = 0;
    globals->friendly_bitvector[2] = 0;
    globals->friendly_bitvector[3] = 0;

    for ( int16_t team = 0; team < 10; ++team )
    {
        int dword_offset = 4 * (((11 * team) >> 5) + 41);
        unsigned int bit = 1u << ((11 * team) & 0x1F);
        *(unsigned int *)((char *)&globals->allegiance_count + dword_offset) |= bit;
    }
}
