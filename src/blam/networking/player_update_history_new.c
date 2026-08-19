/* player_update_history_new @0x837AB3C0 — allocate and zero one player_update_history record.
 *
 * DEVIATION: the decompiler rendered the initialization as a 9-element float store loop; the
 * disassembly stores WORDS at +0x00..+0x14 and single floats at +0x18/+0x1C/+0x20, which is exactly
 * player_update_history (3 int/pointer fields + history_playback_statistics{3 int, 3 float}), and the
 * requested size 0x24 is sizeof(player_update_history). Typed accordingly. */

#include "headers/player_update_history.h"

extern void *dlMalloc(unsigned int size, const char *file, unsigned int line);

player_update_history *player_update_history_new(void)
{
    player_update_history *history = dlMalloc(sizeof(player_update_history),
            "D:\\Projects\\code\\HCEX\\sources\\networking\\player_update_history.c", 0x2Bu);

    history->next_update_id = 0;
    history->update_history_head = nullptr;
    history->update_history_tail = nullptr;
    history->playback_stats.number_of_playbacks = 0;
    history->playback_stats.total_updates_played_back = 0;
    history->playback_stats.total_ticks_played_back = 0;
    history->playback_stats.total_difference_between_client_prediction_and_playback = 0.0f;
    history->playback_stats.average_difference_between_client_prediction_and_playback = 0.0f;
    history->playback_stats.average_ticks_played_back = 0.0f;
    return history;
}
