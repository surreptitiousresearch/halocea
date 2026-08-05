/* multiplayer_game_set_bitmap_for_map @0x8377FD40 — select the UI bitmap (map preview) for the current network
 * game's map by mapping the loaded map's file name to a fixed animation frame index (0..13 for the stock Halo
 * CE multiplayer maps, 19 for anything unrecognized), stored as the widget's current animation frame. No-op
 * when no network game is active. */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_data.h"

extern network_game_data *network_game_get_game(void);
extern char *strstr(const char *haystack, const char *needle);

void multiplayer_game_set_bitmap_for_map(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();
    if ( !game )
        return;

    const char *name = game->map.name;
    int16_t map_index;
    if ( strstr(name, "beavercreek") )        map_index = 0;
    else if ( strstr(name, "sidewinder") )    map_index = 1;
    else if ( strstr(name, "damnation") )     map_index = 2;
    else if ( strstr(name, "ratrace") )       map_index = 3;
    else if ( strstr(name, "prisoner") )      map_index = 4;
    else if ( strstr(name, "hangemhigh") )    map_index = 5;
    else if ( strstr(name, "chillout") )      map_index = 6;
    else if ( strstr(name, "carousel") )      map_index = 7;
    else if ( strstr(name, "boardingaction") )map_index = 8;
    else if ( strstr(name, "bloodgulch") )    map_index = 9;
    else if ( strstr(name, "wizard") )        map_index = 10;
    else if ( strstr(name, "putput") )        map_index = 11;
    else if ( strstr(name, "longest") )       map_index = 12;
    else                                      map_index = strstr(name, "icefields") ? 13 : 19;

    widget->animation_data.current_frame_index = map_index;
}
