/* units_initialize @0x836C6B40 — one-time allocation of the unit globals block from the game-state arena
 * (called from game_initialize). The object_type_definition `initialize` slot is void (*)(void) and no
 * caller consumes r3, so the trailing r3 the decompiler rendered as `return result` is the assignment's
 * leftover, not a return value (B8 wave). */

#include "headers/unit_globals.h"

extern void *game_state_malloc(const char *name, const char *type, int size);

void units_initialize(void)
{
    unit_globals = game_state_malloc("unit globals", 0, 8);
}
