/* hud_print_message @0x836A5880 — post a text message to a local player's HUD message area. Picks the
 * first free message slot (of 4), or the oldest one if all are in use, then stores the text, stamps the
 * game time and a fresh magic number, marks it valid, and clears the leave-first-line-blank flag.
 * message_data is a hud_messaging_datum[2] in the globals, indexed by local player. */

#include <stdint.h>
#include "headers/hud_messaging_globals.h"
#include "headers/hud_messaging_datum.h"

extern void ustrncpy(wchar_t *dest, const wchar_t *src, unsigned int count);
extern int game_time_get(void);

void hud_print_message(int16_t local_player_index, const wchar_t *text)
{
    if ( local_player_index != -1 )
    {
        int16_t oldest_slot = 0;
        int oldest_time = 0x7FFFFFFF;
        hud_messaging_datum *datum = &hud_messaging_globals->message_data[local_player_index];

        hud_message *message;
        int16_t i = 0;
        while ( 1 )
        {
            int16_t current = i;
            message = &datum->messages[i];
            if ( !message->valid )
                break;
            if ( oldest_time > message->time )
            {
                oldest_time = message->time;
                oldest_slot = i;
            }
            ++i;
            if ( (uint16_t)(current + 1) >= 4u )
            {
                message = &datum->messages[oldest_slot];
                break;
            }
        }

        ustrncpy(message->msg, text, 0x3F);
        message->item_definition_index = -1;
        message->time = game_time_get();
        message->valid = 1;
        message->magic_number = hud_messaging_globals->magic_number++;
        datum->leave_first_line_blank = 0;
    }
}
