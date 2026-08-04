#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/slayer_globals.h"

extern void usprintf(wchar_t *string, const wchar_t *format, ...);

uint16_t * slayer_get_score_string(int player_index, uint16_t *buffer)
{
    usprintf(buffer, L"%d", slayer_globals.individual_score[player_index]);
    return buffer;
}
