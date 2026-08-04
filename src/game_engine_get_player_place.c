/* game_engine_get_player_place @0x8374BD20 — looks up `player_index`'s row in the "combined" postgame
 * statistic buffer (statistic 0: score/kills/deaths/assists, see populate_statistic_buffer.c) and copies it
 * into *result.
 *
 * DEVIATION — the decompile's copy tail (`p_place = &result[-1].place; v7 = &v10[v4-1].place;` followed by a
 * 7-iteration `*++p_place = *++v7;` loop) is a raw word-copy of the ENTIRE 28-byte/7-int statistic_buffer
 * (player_index through place) that Hex-Rays rendered via pointer arithmetic anchored one struct short and
 * offset onto the `place` field, relying on pre-increment to walk back onto the struct's true start on the
 * first iteration. Verified field-for-field against statistic_buffer.h's 7-int layout; reproduced as a plain
 * struct assignment. */

#include <stdint.h>
#include "headers/statistic_buffer.h"
#include "headers/postgame_statistic.h"

extern int populate_statistic_buffer(statistic_buffer *statistic_buffer, enum postgame_statistic statistic, uint8_t inverse);

statistic_buffer * game_engine_get_player_place(statistic_buffer *result, int player_index)
{
    statistic_buffer entries[17];
    populate_statistic_buffer(entries, _postgame_statistic_ranking, 0);

    int index = 0;
    while (entries[index].player_index != player_index)
        index++;

    *result = entries[index];
    return result;
}
