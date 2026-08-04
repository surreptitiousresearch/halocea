#include "headers/blam_data_globals.h"
/* player_control_initialize @ 0x836DE2E8 — allocate player-control globals (144 bytes) */

typedef struct player_control_globals_t player_control_globals_t;
extern void *game_state_malloc(const char *name, const char *type, int size);

void player_control_initialize(void)
{
    player_control_globals = game_state_malloc("player control globals", 0, 144);
}
