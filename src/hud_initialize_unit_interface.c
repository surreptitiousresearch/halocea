#include "headers/blam_data_globals.h"
/* hud_initialize_unit_interface @ 0x836D6630 — allocate unit_hud_globals (180 bytes) */

typedef struct unit_hud_globals_definition unit_hud_globals_definition;
extern void *game_state_malloc(const char *name, const char *type, int size);

void hud_initialize_unit_interface(void)
{
    unit_hud_globals = game_state_malloc("hud unit interface", 0, 180);
}
