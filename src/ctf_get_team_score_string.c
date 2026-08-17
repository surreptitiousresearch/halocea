/* ctf_get_team_score_string @0x83806E50 */
#include <stdint.h>
#include "headers/ctf_globals.h"

extern void usprintf(wchar_t *string, const wchar_t *format, ...);

uint16_t * ctf_get_team_score_string(int team_index, uint16_t *buffer)
{
    usprintf(buffer, L"%d", ctf_globals.score[team_index]);
    return buffer;
}
