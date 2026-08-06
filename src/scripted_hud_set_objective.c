/* scripted_hud_set_objective @0x836A40F0 — display a scenario HUD objective message. Looks the message up in
 * the scenario HUD-messages tag (64-byte records); if it is a single-element, non-custom message, it becomes
 * the active objective with an uptime equal to the HUD globals' objective up + fade durations.
 *
 * The HUD globals' objective up/fade tick fields are read by their database offsets (0x11C / 0x11E). */

#include <stdint.h>
#include "headers/scenario.h"
#include "headers/global_tag_instances.h"
#include "headers/hud_messaging_globals.h"
#include "headers/hud_state_message_definition.h"
#include "headers/hud_globals.h"
#include "headers/hud_state_messages.h"
#include "headers/hud_state_message_element.h"
#include "headers/blam_data_globals.h"


void scripted_hud_set_objective(int16_t message_index)
{
    int tag_index = global_scenario->hud_messages.index;
    if ( tag_index == -1 )
        return;

    hud_state_messages *tag_data = TAG_GET(hud_state_messages, tag_index);
    /* recovered: (message_index << 6) + base -> &definitions[message_index]  (stride 64 == sizeof(hud_state_message_definition)) */
    hud_state_message_definition *definitions = (hud_state_message_definition *)tag_data->messages.address;
    hud_state_message_definition *message = &definitions[message_index];

    /* The message qualifies only when its single element's type is zero.
     * DEVIATION: the decompiler flattened the elements block to `unsigned char *` and spelled the
     * 2*index scaling as __ROL4__(x, 1); disasm 0x836A413C-0x836A4140 is rotlwi+lbzx, i.e. a
     * 2-byte hud_state_message_element stride with the byte read at offset 0 (`type`). */
    const hud_state_message_element *elements = (const hud_state_message_element *)tag_data->elements.address;
    unsigned int element_index = message->element_start_index;
    if ( message->element_count == 1 && !elements[element_index].type )
    {
        hud_messaging_globals->objective.message = message;
        /* recovered: *(__int16 *)((char *)hud_globals + 0x11C/0x11E) ->
         * messaging.objective_color.custom.objective.up_ticks/fade_ticks */
        int16_t up_ticks = hud_globals->messaging.objective_color.custom.objective.up_ticks;
        int16_t fade_ticks = hud_globals->messaging.objective_color.custom.objective.fade_ticks;
        hud_messaging_globals->objective.uptime = fade_ticks + up_ticks;
    }
}
