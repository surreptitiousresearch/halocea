/* multiplayer_game_set_bitmap_for_ruleset @0x8377FF5C — sets a widget's animation frame index to the
 * ruleset bitmap frame matching the active network game's game engine (falls back to a generic frame if
 * there's no active game, or the engine index is out of the known range). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_data.h"
#include "headers/game_engine_type.h"

extern network_game_data *network_game_get_game(void);

void multiplayer_game_set_bitmap_for_ruleset(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();
    if ( !game )
        return;

    int16_t frame_index;
    if ( (unsigned int)(game->variant.game_engine_index - 1) > 4 )
    {
        frame_index = 5;
    }
    else switch ( game->variant.game_engine_index )
    {
        case game_engine_ctf: frame_index = 0; break;
        case game_engine_slayer: frame_index = 2; break;
        case game_engine_oddball: frame_index = 3; break;
        case game_engine_king: frame_index = 1; break;
        default: frame_index = 4; break;
    }

    widget->animation_data.current_frame_index = frame_index;
}
