/* game_allegiance_remove @0x83745BC0 — remove the explicit allegiance between two teams (either ordering). The
 * matching record is marked broken, the ally/friendly bit-matrix entries for both orderings are cleared, AI
 * props are updated (permanent break), and the record is swap-removed from the table. Returns 1 if a record
 * was removed, 0 otherwise. */

#include <stdint.h>
#include "headers/bit_vector.h"
#include "headers/game_allegiance.h"

extern void ai_handle_allegiance_status_changed(int16_t team1_index, int16_t team2_index, uint8_t broken, uint8_t permanently_broken);

int game_allegiance_remove(int16_t team1_index, int16_t team2_index)
{
    struct_game_allegiance_globals *globals = game_allegiance_globals;
    game_allegiance *allegiance = globals->allegiances;
    int count = globals->allegiance_count;
    int index = 0;

    if ( count <= 0 )
        return 0;

    while ( 1 )
    {
        int t1 = allegiance->team1_index;
        if ( t1 == team1_index && allegiance->team2_index == team2_index )
            break;
        if ( allegiance->team2_index == team1_index && t1 == team2_index )
            break;
        ++allegiance;
        index = (int16_t)(index + 1);
        if ( index >= count )
            return 0;
    }

    allegiance->currently_broken = 1;

    int t1 = allegiance->team1_index;
    int t2 = allegiance->team2_index;
    if ( t1 < 10 && t2 < 10 )
    {
        int ab = 10 * t1 + t2;
        int ba = 10 * t2 + t1;
        BIT_VECTOR_CLEAR_FLAG(globals->ally_bitvector, ab);
        BIT_VECTOR_CLEAR_FLAG(globals->ally_bitvector, ba);
        BIT_VECTOR_CLEAR_FLAG(globals->friendly_bitvector, ab);
        BIT_VECTOR_CLEAR_FLAG(globals->friendly_bitvector, ba);
    }

    allegiance->status_changed = 1;
    ai_handle_allegiance_status_changed(allegiance->team1_index, allegiance->team2_index, 1, 1);

    /* swap-remove: decrement count, move the (new) last record into the freed slot. Decompiler does this with a
     * 9-word int16 copy loop; reproduced as the equivalent struct assignment. */
    int16_t new_count = game_allegiance_globals->allegiance_count - 1;
    game_allegiance_globals->allegiance_count = new_count;
    if ( new_count > (int16_t)index )
        game_allegiance_globals->allegiances[index] = game_allegiance_globals->allegiances[new_count];

    return 1;
}
