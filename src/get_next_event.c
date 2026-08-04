/* get_next_event @ 0x83726420 — pop the most-recent pending event for a local player
 * (or any player when local_player_index == -1) out of the event queue. Returns 1 and fills
 * *event if one was found, consuming it; 0 otherwise. */

#include <stdint.h>
#include "headers/event_manager_globals.h"

uint8_t get_next_event(event_record *event, int16_t local_player_index)
{
    uint8_t result = 0;

    if ( !event_manager_globals.initialized )
        return result;

    if ( local_player_index == -1 )
    {
        int16_t player;
        for ( player = 0; player < 4; ++player )
        {
            result = get_next_event(event, player);
            if ( result )
                break;
        }
        return result;
    }

    /* scan this player's queue from newest (slot 7) to oldest */
    {
        int slot;
        for ( slot = 7; slot >= 0; --slot )
        {
            event_record *queued = &event_manager_globals.event_queue[local_player_index][slot];
            if ( queued->type )
            {
                event->type = queued->type;
                event->controller_index = queued->controller_index;
                event->value = queued->value;
                queued->type = 0;
                return 1;
            }
        }
    }
    return result;
}
