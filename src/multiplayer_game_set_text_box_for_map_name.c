/* multiplayer_game_set_text_box_for_map_name @0x8377F8E8 — set a UI text box to display the current network
 * game's map by mapping the loaded map's file name to a fixed string-list index (0..13 for the stock Halo CE
 * multiplayer maps, 19 for anything unrecognized). No-op when no network game is active. */

#include "headers/widget_instance.h"
#include "headers/network_game_data.h"

extern network_game_data *network_game_get_game(void);
extern char *strstr(const char *haystack, const char *needle);

void multiplayer_game_set_text_box_for_map_name(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();
    if ( !game )
        return;

    const char *name = game->map.name;
    __int16 map_index;
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

    widget->parameters.text_box_parameters.string_list_index = map_index;
}
