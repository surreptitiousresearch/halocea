/* postgame_statistic_get_rating @0x8374BDA0 — the given player's rank (0-based, ties share a rank) within
 * the sorted per-player rows for `statistic`. Returns 0 if the player holds the top row already, or if
 * only one row exists.
 *
 * DEVIATION: disasm_range(0x8374BDA0,0x8374BE24), cross-checked against raw instruction operands (the
 * decompiler's own two-piece stack-var split obscured the true field offsets), shows every load in the loop
 * landing on the `score` field (offset 4) and `player_index` field (offset 0) of consecutive
 * statistic_buffer rows — the decompiler's "v7/v8" names/offsets did not correspond to real fields. */

#include <stdint.h>
#include "headers/statistic_buffer.h"
#include "headers/postgame_statistic.h"

extern int populate_statistic_buffer(statistic_buffer *statistic_buffer, enum postgame_statistic statistic, uint8_t inverse);

int postgame_statistic_get_rating(int player_index, enum postgame_statistic statistic, uint8_t inverse)
{
    statistic_buffer entries[17];
    int count = populate_statistic_buffer(entries, statistic, inverse);
    int rank = 0;

    if ( entries[0].player_index != player_index && count > 1 )
    {
        for ( int i = 1; i < count; ++i )
        {
            if ( entries[i - 1].score != entries[i].score )
                ++rank;
            if ( entries[i].player_index == player_index )
                break;
        }
    }

    return rank;
}
