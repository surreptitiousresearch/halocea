/* hud_set_state_text @0x836A4900 — copy a caller-supplied unicode string into a local player's HUD
 * state-message buffer (bounded to 255 chars) and force-terminate the final slot.
 *
 * DEVIATION: the decompiler addresses the buffer via a raw unsigned short* base + short indices
 * (v2 + 280, v2[535]); those resolve to hud_messaging_datum.state_message.message_buffer and its
 * last element, which are the named fields used here. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/hud_messaging_datum.h"

extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);

void hud_set_state_text(int16_t local_player_index, const wchar_t *message)
{
    hud_messaging_datum *datum = &hud_messaging_globals->message_data[local_player_index];

    ustrncpy(datum->state_message.message_buffer, message, 0xFFu);
    datum->state_message.message_buffer[255] = 0;
}
