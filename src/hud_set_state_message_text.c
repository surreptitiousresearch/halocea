/* hud_set_state_message_text @0x836A47F0 — override one element slot of a local player's active HUD
 * state message with a text reference (string index + scenario-names flag), setting that slot's
 * icon-vs-text bit. No-op unless the state message is valid, a source definition is bound, and
 * scripted HUD help text is not showing. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/hud_scripted_globals.h"
#include "headers/hud_messaging_datum.h"

void hud_set_state_message_text(
        int16_t local_player_index,
        int16_t custom_icon_index,
        int16_t icon_string_index,
        char uses_scenario_names)
{
    hud_messaging_datum *datum = &hud_messaging_globals->message_data[local_player_index];

    if ( datum->state_message.valid && !hud_scripted_globals->show_hud_help_text )
    {
        if ( datum->state_message.state_message )
        {
            datum->state_message.info[custom_icon_index].text.string_index = icon_string_index;
            datum->state_message.info[custom_icon_index].text.uses_scenario_names = uses_scenario_names;
            datum->state_message.is_text_flags |= 1 << custom_icon_index;
        }
    }
}
