#include "headers/widget_instance.h"
#include "headers/network_game_data.h"

extern network_game_data *network_game_get_game(void);

void teams_no_teams_mp_game_bitmap_update(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();

    if (game)
        widget->animation_data.current_frame_index = game->variant.universal_variant.teams != 1;
}
