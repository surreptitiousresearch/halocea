#include "headers/blam_data_globals.h"
/* breakable_surfaces_initialize @ 0x83743000 — allocate breakable-surface globals (16900 bytes) */

typedef struct breakable_surface_globals breakable_surface_globals;
extern void *game_state_malloc(const char *name, const char *type, int size);

void breakable_surfaces_initialize(void)
{
    globals = game_state_malloc("breakable surface globals", 0, 16900);
}
