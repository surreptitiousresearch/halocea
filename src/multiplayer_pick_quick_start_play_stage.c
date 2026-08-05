/* multiplayer_pick_quick_start_play_stage @0x836923F0 — menu handler: initialize the game-engine
 * playlist, advance it to the next entry, and mark the network game as a local quick-start. Always
 * reports the event handled. (game_engine_playlist_next / network_game_set_quickstart_local take no
 * arguments; the decompiler's threaded arguments are register-leftover noise.) */

#include <stdint.h>
#include "headers/widget_instance.h"
#include "headers/event_record.h"

extern int game_engine_playlist_initialize(uint8_t force_begin_playlist);
extern void game_engine_playlist_next(void);
extern void network_game_set_quickstart_local(void);

uint8_t multiplayer_pick_quick_start_play_stage(widget_instance *widget, event_record *event, uint8_t *widget_deleted)
{
    game_engine_playlist_initialize(0);
    game_engine_playlist_next();
    network_game_set_quickstart_local();
    return 1;
}
