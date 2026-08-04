/* multiplayer_game_set_text_box_for_number_of_players @0x8377FFD8 — stamp the current game's player count
 * into the widget's text-box buffer (reallocated to fit, 3 digits + terminator). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_data.h"

extern network_game_data *network_game_get_game(void);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);

void multiplayer_game_set_text_box_for_number_of_players(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();

    if ( game )
    {
        unsigned __int16 *text = ui_widget_realloc(widget->parameters.text_box_parameters.text, 8u);

        widget->parameters.text_box_parameters.text = text;
        if ( text )
        {
            usnprintf(text, 3u, L"%d", game->player_count);
            widget->parameters.text_box_parameters.text[3] = 0;
        }
    }
}
