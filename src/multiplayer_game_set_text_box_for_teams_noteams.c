#include "headers/widget_instance.h"
#include "headers/network_game_data.h"
#include "headers/multiplayer_game_text_string.h"

extern network_game_data *network_game_get_game(void);

void multiplayer_game_set_text_box_for_teams_noteams(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();

    if (game)
        widget->parameters.text_box_parameters.string_list_index =
            (game->variant.universal_variant.teams != 1) ? _string_free_for_all : _string_team_game;
}
