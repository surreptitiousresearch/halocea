/* hud_enable_custom_state_message @0x836A4868 — turn a local player's HUD state message into a
 * script-driven custom message. Sets validity to the requested state, drops any bound source
 * definition, and (when enabling) seeds the message buffer with the empty wide string. Any change of
 * validity forces leave_first_line_blank so the line re-lays out. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/hud_messaging_datum.h"

extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern wchar_t empty_wide_string;   /* empty wide string */

void hud_enable_custom_state_message(int16_t local_player_index, char enabled)
{
    hud_messaging_datum *datum = &hud_messaging_globals->message_data[local_player_index];
    int previous_valid = datum->state_message.valid;
    char leave_first_line_blank = datum->leave_first_line_blank;

    datum->state_message.valid = enabled;
    datum->state_message.state_message = 0;
    datum->leave_first_line_blank = ((uint8_t)enabled != previous_valid) | leave_first_line_blank;
    if ( enabled )
    {
        datum->state_message.state_message = 0;
        ustrncpy(datum->state_message.message_buffer, &empty_wide_string, 0xFFu);
    }
    datum->custom_message = enabled;
}
