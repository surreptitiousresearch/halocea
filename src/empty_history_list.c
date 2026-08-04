/* empty_history_list @0x837ABDC8 — free every queued update in a player_update_history and reset the
 * list to empty. Walks the singly-linked update chain, releasing each node, then clears head and tail. */

#include <stddef.h>
#include "headers/player_update_history.h"
#include "headers/player_update.h"

extern void dlFree(void *ptr);

void empty_history_list(player_update_history *history)
{
    player_update *update = history->update_history_head;
    while ( update )
    {
        player_update *next = update->next;
        dlFree(update);
        update = next;
    }
    history->update_history_head = NULL;
    history->update_history_tail = NULL;
}
