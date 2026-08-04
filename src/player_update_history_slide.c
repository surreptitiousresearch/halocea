/* player_update_history_slide @0x837AB420 — find the update following `last_completed_id` in a player's
 * update-history list and return it; when `destructive` and the id was found, also free every node up to
 * and including the completed one and re-head the list there (clearing the tail when the list empties). */

#include <stdint.h>
#include "headers/player_update_history.h"
#include "headers/player_update.h"

extern void dlFree(void *ptr);

player_update * player_update_history_slide(player_update_history *history, int last_completed_id, uint8_t destructive)
{
    player_update *head = history->update_history_head;
    player_update *next = 0;
    unsigned __int8 found = 0;

    for ( player_update *update = head; update; update = update->next )
    {
        if ( update->id == last_completed_id )
        {
            next = update->next;
            found = 1;
            break;
        }
    }

    if ( found != 1 || destructive != 1 )
        return next;

    while ( head )
    {
        player_update *following = head->next;
        int was_completed = head->id == last_completed_id;
        dlFree(head);
        head = following;
        if ( was_completed )
            break;
    }

    history->update_history_head = head;
    if ( !head )
        history->update_history_tail = 0;
    return next;
}
