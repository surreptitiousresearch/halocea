/* multiplayer_game_set_text_box_for_game_ruleset @0x8377FAF0 — sets a widget's text-box string to the
 * ruleset description matching the active network game's game engine and variant-specific sub-mode
 * (falls back to a generic string if there's no active game, or the engine index is out of the known
 * range). Counterpart of multiplayer_game_set_bitmap_for_ruleset.c (which only distinguishes the engine,
 * not the sub-mode). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_data.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_game_text_string.h"

extern network_game_data *network_game_get_game(void);

void multiplayer_game_set_text_box_for_game_ruleset(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();
    if ( !game )
        return;

    int16_t string_index;
    if ( (unsigned int)(game->variant.game_engine_index - 1) > 4 )
    {
        string_index = _string_unknown_game_type;
    }
    else switch ( game->variant.game_engine_index )
    {
    case game_engine_slayer:
        string_index = _string_slayer;
        break;
    case game_engine_oddball:
        switch ( game->variant.game_engine_variant.oddball.oddball_ball_type )
        {
        case _oddball_magic:
            string_index = _string_reverse_tag;
            break;
        case _oddball_terminator:
            string_index = _string_juggernaut;
            break;
        default:
            string_index = _string_oddball;
            break;
        }
        break;
    case game_engine_king:
        string_index = _string_king_of_the_hill;
        break;
    case game_engine_ctf:
        if ( game->variant.game_engine_variant.ctf.assault == 1 )
            string_index = game->variant.game_engine_variant.ctf.single_flag_time
                    ? _string_single_flag_assault : _string_assault;
        else
            string_index = game->variant.game_engine_variant.ctf.single_flag_time
                    ? _string_single_flag_ctf : _string_capture_the_flag;
        break;
    default: /* game_engine_race */
        string_index = game->variant.game_engine_variant.terminator.ignored == 2
                ? _string_rally : _string_race;
        break;
    }

    widget->parameters.text_box_parameters.string_list_index = string_index;
}
