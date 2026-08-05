/* hud_messaging_get_objective @0x836A4950 — return a pointer to the wide-string text for the active
 * HUD objective message, or null when none is set. Resolves the objective's source definition to its
 * text_start_index within the scenario HUD-messages tag's text_data block.
 *
 * DEVIATION: the decompiler renders the tag text pointer as (...)[3] (tag_data.address at 0x0C) and
 * the char offset as __ROL4__(text_start_index, 1) (a rotate that, for the u16 index, is text_start_index
 * * 2 — i.e. one wchar_t per index). Both are expressed via named fields here. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/hud_state_message_definition.h"
#include "headers/scenario.h"
#include "headers/hud_state_messages.h"
#include "headers/global_tag_instances.h"

uint16_t * hud_messaging_get_objective(void)
{
    hud_state_message_definition *message;
    hud_state_messages *messages_tag;

    message = hud_messaging_globals->objective.message;
    if ( !message )
        return 0;

    messages_tag = TAG_GET(hud_state_messages, global_scenario->hud_messages.index);
    return (uint16_t *)messages_tag->text_data.address + message->text_start_index;
}
