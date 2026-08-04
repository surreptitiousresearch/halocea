/* rasterizer_screen_effects_initialize @ 0x83724208
   Allocates the 120-byte cinematic screen-effect block from the game-state
   arena and publishes it to the global. */

#include "headers/cinematic_screen_effect_globals.h"

extern void *game_state_malloc(const char *name, const char *type, int size);

/* return attested void: r3 at blr is game_state_malloc threading (stw to the global
   @0x8372422C is the real sink); 0/1 sampled callers consume r3 (_rasterizer_initialize
   @0x8369BD18 ignores) — decompiler r3-status threading killed per protocol */
void rasterizer_screen_effects_initialize(void)
{
    cinematic_screen_effect_globals =
        game_state_malloc("screen effect filth", 0, 120);
}
