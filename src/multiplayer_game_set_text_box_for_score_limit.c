/* multiplayer_game_set_text_box_for_score_limit @0x8377FC40 — stamp the current game variant's score-to-win
 * limit into the widget's text-box buffer (reallocated to fit, 7 digits + terminator). */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/network_game_data.h"

extern network_game_data *network_game_get_game(void);
extern void * ui_widget_realloc(void *ptr, uint16_t size);
extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);

void multiplayer_game_set_text_box_for_score_limit(widget_instance *widget)
{
    network_game_data *game = network_game_get_game();

    if ( game )
    {
        uint16_t *text = ui_widget_realloc(widget->parameters.text_box_parameters.text, 0x10u);

        widget->parameters.text_box_parameters.text = text;
        if ( text )
        {
            usnprintf(text, 7u, L"%d", game->variant.universal_variant.score_to_win);
            widget->parameters.text_box_parameters.text[7] = 0;
        }
    }
}
