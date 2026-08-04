/* player_update_history_clear @0x837AC2C8 — discard a player's queued movement-update history.
 * Thin wrapper: forwards directly to empty_history_list. */

struct player_update_history;

#include "headers/player_update_history.h"
extern void empty_history_list(player_update_history *history);

void player_update_history_clear(struct player_update_history *history)
{
    empty_history_list(history);
}
