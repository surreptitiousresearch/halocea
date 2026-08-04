#include "headers/blam_data_globals.h"
/* structure_decals_initialize @ 0x837E2CF8 — allocate structure-decal globals (4 bytes).
 * Return: void — the sole caller (structures_initialize) ignores r3; the pointer left in r3
 * at blr is game_state_malloc's tail residue, not a declared return value. */

extern void *game_state_malloc(const char *name, const char *type, int size);

void structure_decals_initialize(void)
{
    structure_decals_globals = game_state_malloc("structure decals", 0, 4);
}
