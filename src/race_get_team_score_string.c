#include <stdint.h>
#include "headers/race_globals.h"
#include <stddef.h>   /* wchar_t */

extern void usprintf(wchar_t *string, const wchar_t *format, ...);

uint16_t * race_get_team_score_string(int team_index, uint16_t *buffer)
{
    usprintf(buffer, L"%d", race_globals.team_laps[team_index]);
    return buffer;
}
