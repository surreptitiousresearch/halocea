#include "headers/blam_data_globals.h"
/* vibrate_initialize @ 0x83739118 — allocate controller-vibrate globals (1052 bytes) */

typedef struct vibrate_global_data vibrate_global_data;
extern void *game_state_malloc(const char *name, const char *type, int size);

void vibrate_initialize(void)
{
    vibrate_globals = game_state_malloc("vibrate", 0, 1052);
}
