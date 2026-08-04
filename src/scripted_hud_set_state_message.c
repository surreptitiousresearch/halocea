/* scripted_hud_set_state_message @0x836A3FD8 — set the persistent HUD help/state message from the scenario
 * HUD-messages tag (64-byte records), but only while scripted HUD help text is enabled. */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/hud_messaging_globals.h"
#include "headers/hud_state_message_definition.h"
#include "headers/hud_state_messages.h"
#include "headers/hud_scripted_globals.h"
#include "headers/blam_data_globals.h"


void scripted_hud_set_state_message(int16_t message_index)
{
    if ( !hud_scripted_globals->show_hud_help_text )
        return;

    int tag_index = global_scenario->hud_messages.index;
    if ( tag_index == -1 )
        return;

    /* stride 64 == sizeof(hud_state_message_definition); messages block at hud_state_messages+36 */
    hud_state_message_definition *definitions =
        (hud_state_message_definition *)(TAG_GET(hud_state_messages, tag_index))->messages.address;
    hud_messaging_globals->help_message = &definitions[message_index];
}
