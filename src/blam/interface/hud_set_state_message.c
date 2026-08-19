/* hud_set_state_message @0x836A46E0 — select the persistent HUD "state message" definition shown for
 * a local player. Looks up message_index in the global HUD-messages tag; a valid index binds the
 * source definition (clearing the per-element icon/text flags), while -1 (or an out-of-range index)
 * marks the state message invalid. No-op while scripted HUD help text is showing. */

#include <stdint.h>
#include "headers/hud_globals.h"
#include "headers/hud_messaging_globals.h"
#include "headers/hud_scripted_globals.h"
#include "headers/hud_messaging_datum.h"
#include "headers/hud_state_messages.h"
#include "headers/hud_state_message_definition.h"
#include "headers/global_tag_instances.h"

void hud_set_state_message(int16_t local_player_index, int16_t message_index)
{
    int hud_messages_tag_index;
    hud_messaging_datum *datum;
    hud_state_messages *messages_tag;

    if ( hud_scripted_globals->show_hud_help_text )
        return;

    hud_messages_tag_index = hud_globals->messaging.hud_messages.index;
    if ( hud_messages_tag_index == -1 )
        return;

    datum = &hud_messaging_globals->message_data[local_player_index];
    if ( message_index != -1 )
    {
        messages_tag = TAG_GET(hud_state_messages, hud_messages_tag_index);
        if ( message_index >= messages_tag->messages.count )
        {
            message_index = -1;
        }
        else
        {
            datum->state_message.is_text_flags = 0;
            datum->state_message.state_message =
                (hud_state_message_definition *)messages_tag->messages.address + message_index;
        }
    }
    datum->state_message.valid = message_index != -1;
}
