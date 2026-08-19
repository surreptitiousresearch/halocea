/* hud_initialize_weapon_interface_for_new_map @0x836AE870 — reset the weapon-HUD runtime state (all -1). */

#include <string.h>
#include "headers/weapon_hud_globals.h"

/* Tail-call `b memset`; sole caller hud_initialize_for_new_map ignores r3 => void return
 * (the void* is TCO residue, not a real value). */
void hud_initialize_weapon_interface_for_new_map(void)
{
    memset(weapon_hud_globals, -1, sizeof(weapon_hud_globals_definition));
}
