#include "headers/player_update_history.h"
#include "headers/player_update.h"

void get_history_list_length_stats(const player_update_history *history, int *number_of_updates, int *number_of_ticks)
{
    *number_of_updates = 0;
    *number_of_ticks = 0;

    for (player_update *update = history->update_history_head; update; update = update->next)
    {
        ++*number_of_updates;
        *number_of_ticks += update->ticks_to_apply_update_to; /* recovered: *(int *)((char *)update + 4) -> ticks_to_apply_update_to */
    }
}
