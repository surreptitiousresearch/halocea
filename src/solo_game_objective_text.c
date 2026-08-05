/* solo_game_objective_text @0x8377F558 — copy the current HUD objective string into the widget's text-box
 * buffer (reallocated to fit), if there is one. */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */
#include "headers/widget_instance.h"

extern uint16_t * hud_messaging_get_objective(void);
extern unsigned int ustrlen(const wchar_t *string);
extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern void * ui_widget_realloc(void *ptr, uint16_t size);

void solo_game_objective_text(widget_instance *widget)
{
    const wchar_t *objective = (const wchar_t *)hud_messaging_get_objective();

    if ( objective && *objective )
    {
        unsigned int length = ustrlen(objective);

        if ( length > 0 )
        {
            uint16_t *text = ui_widget_realloc(widget->parameters.text_box_parameters.text, 2 * length + 2);

            widget->parameters.text_box_parameters.text = text;
            if ( text )
            {
                ustrncpy(text, objective, length);
                widget->parameters.text_box_parameters.text[length] = 0;
            }
        }
    }
}
