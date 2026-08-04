/* hud_set_state_message_icon @0x836A4780 — override one element slot of a local player's active HUD
 * state message with a custom icon element (clearing that slot's icon-vs-text bit). No-op unless the
 * state message is valid, a source definition is bound, and scripted HUD help text is not showing. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/hud_scripted_globals.h"
#include "headers/hud_messaging_datum.h"
#include "headers/icon_hud_element_definition.h"

void hud_set_state_message_icon(int16_t local_player_index, int16_t custom_icon_index, const icon_hud_element_definition *icon)
{
    hud_messaging_datum *datum = &hud_messaging_globals->message_data[local_player_index];

    if ( datum->state_message.valid && !hud_scripted_globals->show_hud_help_text )
    {
        if ( datum->state_message.state_message )
        {
            datum->state_message.info[custom_icon_index].icon = icon;
            datum->state_message.is_text_flags &= ~(1 << custom_icon_index);
        }
    }
}
