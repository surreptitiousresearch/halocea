/* multiplayer_game_set_text_box_for_score_limit_type @0x8377FCCC — picks the string describing what kind
 * of "score limit" the current game variant uses (points, time, kills, etc.), keyed off the variant's game
 * engine and, for king/oddball, sub-mode. */

#include "headers/widget_instance.h"
#include "headers/network_game_data.h"
#include "headers/game_engine_type.h"
#include "headers/multiplayer_game_text_string.h"

extern network_game_data *network_game_get_game(void);

void multiplayer_game_set_text_box_for_score_limit_type(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();
    if ( !game )
        return;

    int game_engine_index = game->variant.game_engine_index;
    __int16 string_list_index;

    if ( game_engine_index >= first_usable_game_engine_index
      && game_engine_index <= last_usable_game_engine_index
      && game_engine_index != game_engine_slayer )
    {
        if ( game_engine_index == game_engine_oddball
             && game->variant.game_engine_variant.oddball.oddball_ball_type == _oddball_terminator )
            string_list_index = _string_frags;
        else if ( game_engine_index == game_engine_oddball || game_engine_index == game_engine_king )
            string_list_index = _string_minutes;
        else if ( game_engine_index == game_engine_ctf )
            string_list_index = _string_captures;
        else
            string_list_index = _string_laps;
    }
    else
    {
        string_list_index = _string_frags;
    }

    widget->parameters.text_box_parameters.string_list_index = string_list_index;
}
