/* players_initialize @ 0x836A78A8 — player/team pools + players globals */

#include <stdint.h>
#include "headers/data_array.h"
#include "headers/players_global_data.h"
#include "headers/blam_data_globals.h"

#include "headers/data_array.h"
extern data_array *game_state_data_new(const char *name, int16_t maximum_count, int16_t size);
extern void *game_state_malloc(const char *name, const char *type, int size);
extern void player_control_initialize(void);

void players_initialize(void)
{
    player_data = game_state_data_new("players", 32, 512);
    team_data = game_state_data_new("teams", 32, 64);
    players_globals = game_state_malloc("players globals", 0, 160);
    players_globals->local_player_indices[0] = -1;
    players_globals->local_player_indices[1] = -1;
    players_globals->local_player_network_identifier = -1;
    players_globals->local_player_count = 0;
    player_control_initialize(); /* attested void: r3-thread + cast dropped */
}
