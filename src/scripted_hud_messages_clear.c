/* scripted_hud_messages_clear @0x836A4AB0 — clear the valid flag (hud_message +0x82) of all four queued
 * messages in both HUD messaging data slots, dismissing any on-screen scripted messages. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/blam_data_globals.h"


void scripted_hud_messages_clear(void)
{
    for ( int datum = 0; datum < 2; datum = (int16_t)(datum + 1) )
    {
        for ( int message = 0; message < 4; message = (int16_t)(message + 1) )
        {
            /* recovered: message_data + 1120*datum + 140*message, [130] -> messages[].valid */
            hud_messaging_globals->message_data[datum].messages[message].valid = 0;
        }
    }
}
