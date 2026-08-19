/* slayer_get_team_score_string @0x83815878 */
#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/slayer_globals.h"

extern void usprintf(wchar_t *string, const wchar_t *format, ...);

uint16_t * slayer_get_team_score_string(int team_index, uint16_t *buffer)
{
    usprintf(buffer, L"%d", slayer_globals.team_score[team_index]);
    return buffer;
}
