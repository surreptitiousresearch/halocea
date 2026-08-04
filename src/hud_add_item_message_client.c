/* hud_add_item_message_client @0x836A596C — add (or merge into) a HUD item-pickup message for a local
 * player: find a free/matching message slot, accumulate the quantity, and stamp it with the current
 * time and a fresh magic number so it animates in.
 *
 * DEVIATION: the decompiler loses the binding of the item_definition_index/message_offset stores to
 * the parameters (renders them as uninitialized v6/v7); they are the same values passed to
 * find_free_hud_message just above. */

#include "headers/hud_messaging_globals.h"
#include "headers/hud_messaging_datum.h"
#include "headers/hud_message.h"

extern hud_message *find_free_hud_message(hud_messaging_datum *datum, int item_definition_index, char message_offset);
extern int game_time_get(void);

void hud_add_item_message_client(short local_player_index, int item_definition_index, short quantity, char message_offset)
{
    if ( local_player_index != -1 )
    {
        hud_messaging_datum *datum = &hud_messaging_globals->message_data[local_player_index];
        hud_message *message = find_free_hud_message(datum, item_definition_index, message_offset);
        if ( !message->valid )
            message->quantity = 0;
        short accumulated_quantity = message->quantity;
        message->item_definition_index = item_definition_index;
        message->message_offset = message_offset;
        message->quantity = accumulated_quantity + quantity;
        message->time = game_time_get();
        message->valid = 1;
        message->magic_number = hud_messaging_globals->magic_number++;
        datum->leave_first_line_blank = 0;
    }
}
