/* get_variant_name_id @0x83749968 — maps the active network game variant to its display-name index in
 * the multiplayer_game_text string list. Falls back to _string_unknown_game_type when there's no active
 * network game or the engine index is out of the known range. */

#include <stdint.h>
#include "headers/network_game_data.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_game_text_string.h"

extern network_game_data *network_game_get_game(void);

int16_t get_variant_name_id(void)
{
    network_game_data *game = network_game_get_game();
    if (!game)
        return _string_unknown_game_type;

    if ((unsigned int)(game->variant.game_engine_index - 1) > 4)
        return _string_unknown_game_type;

    switch (game->variant.game_engine_index)
    {
    case game_engine_ctf:
        if (game->variant.game_engine_variant.ctf.assault == 1)
            return game->variant.game_engine_variant.ctf.single_flag_time == 0
                ? _string_assault : _string_single_flag_assault;
        return game->variant.game_engine_variant.ctf.single_flag_time == 0
            ? _string_capture_the_flag : _string_single_flag_ctf;
    case game_engine_slayer:
        return _string_slayer;
    case game_engine_oddball:
        switch (game->variant.game_engine_variant.oddball.oddball_ball_type)
        {
        case _oddball_magic:
            return _string_reverse_tag;
        case _oddball_terminator:
            return _string_juggernaut;
        default:
            return _string_oddball;
        }
    case game_engine_king:
        return _string_king_of_the_hill;
    default: /* game_engine_race */
        if (game->variant.game_engine_variant.terminator.ignored == 2)
            return _string_rally;
        return _string_race;
    }
}
