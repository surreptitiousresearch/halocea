#include "headers/blam_data_globals.h"
/* hud_initialize_weapon_interface @ 0x836AE838 — allocate weapon_hud_globals (244 bytes) */

typedef struct weapon_hud_globals_definition weapon_hud_globals_definition;
extern void *game_state_malloc(const char *name, const char *type, int size);

void hud_initialize_weapon_interface(void)
{
    weapon_hud_globals = game_state_malloc("hud weapon interface", 0, 244);
}
