/* oddball_get_team_score_string @0x83816BB0 — formats a team's oddball score into `buffer`: a raw number
 * for oddball-terminator variants, or a ticks-to-time string otherwise. Mirrors oddball_get_score_string.c. */

#include <stdint.h>
#include "headers/oddball_globals.h"
#include "headers/game_variant.h"

extern game_variant *game_engine_get_variant(void);
extern void usprintf(wchar_t *string, const wchar_t *format, ...);
extern void ticks_to_unicode_time_string(int ticks, unsigned int count, uint16_t *buffer);

uint16_t * oddball_get_team_score_string(int team_index, uint16_t *buffer)
{
    int score = oddball_globals.team_score[team_index];

    if ( game_engine_get_variant()->game_engine_variant.oddball.oddball_ball_type == _oddball_terminator )
        usprintf(buffer, L"%d", score);
    else
        ticks_to_unicode_time_string(score, 256, buffer);

    return buffer;
}
