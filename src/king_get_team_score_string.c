#include <stdint.h>
#include "headers/king_globals.h"

extern void ticks_to_unicode_time_string(int ticks, unsigned int count, unsigned __int16 *buffer);

uint16_t * king_get_team_score_string(int team_index, uint16_t *buffer)
{
    ticks_to_unicode_time_string(king_globals.score[team_index], 256, buffer);
    return buffer;
}
