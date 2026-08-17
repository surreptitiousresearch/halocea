/* player_update_history_delete @0x837ABE28 */
#include "headers/player_update_history.h"

extern void empty_history_list(player_update_history *history);
extern void dlFree(void *ptr);

void player_update_history_delete(player_update_history *history)
{
    empty_history_list(history);
    dlFree(history);
}
