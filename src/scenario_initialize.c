#include "headers/blam_data_globals.h"
/* scenario_initialize @ 0x83703320 — allocate scenario globals (168 bytes) */

typedef struct scenario_global_data scenario_global_data;
extern void *game_state_malloc(const char *name, const char *type, int size);

void scenario_initialize(void)
{
    scenario_globals = game_state_malloc("scenario globals", 0, 168);
}
