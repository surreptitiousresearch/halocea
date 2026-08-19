#include "headers/blam_data_globals.h"
/* player_effect_initialize @ 0x837374D8 — allocate player-effect globals (532 bytes) */

typedef struct player_effect_globals_definition player_effect_globals_definition;
extern void *game_state_malloc(const char *name, const char *type, int size);

void player_effect_initialize(void)
{
    player_effect_globals = game_state_malloc("player effects", 0, 532);
}
