/* oddball_get_score_string @0x83816AB0 — formats a player's oddball individual score into `buffer`: a raw
 * number for oddball-terminator variants, or a ticks-to-time string otherwise. */

#include <stdint.h>
#include "headers/oddball_globals.h"
#include "headers/game_variant.h"

extern game_variant *game_engine_get_variant(void);
extern void usprintf(wchar_t *string, const wchar_t *format, ...);
extern void ticks_to_unicode_time_string(int ticks, unsigned int count, unsigned __int16 *buffer);

uint16_t * oddball_get_score_string(int player_index, uint16_t *buffer)
{
    int score = oddball_globals.individual_score[player_index];

    if ( game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type == _oddball_terminator )
        usprintf(buffer, L"%d", score);
    else
        ticks_to_unicode_time_string(score, 256, buffer);

    return buffer;
}
