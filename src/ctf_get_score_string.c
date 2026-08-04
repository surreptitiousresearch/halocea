#include <stdint.h>
#include "headers/data_array.h"
#include "headers/player_datum.h"
#include "headers/blam_data_globals.h"

extern void usprintf(wchar_t *string, const wchar_t *format, ...);

uint16_t * ctf_get_score_string(int player_index, uint16_t *buffer)
{
    player_datum *player = DATA_ARRAY_ELEMENT(player_data, player_datum, player_index);

    /* recovered: *(__int16 *)((char *)player + 200) -> statistics.multiplayer_statistics.ctf_statistics.flag_scores */
    usprintf(buffer, L"%d", player->statistics.multiplayer_statistics.ctf_statistics.flag_scores);
    return buffer;
}
