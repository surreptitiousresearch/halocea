/* game_engine_dispose_from_old_map @ 0x837461D8 — call the active engine's per-map dispose
 * slot. game_engine vtable `dispose_from_old_map` at offset 0x10 (per DB disasm).
 * ATTESTED void: no r3 def on any path, 0 sampled consumers; tail-bctr into a void slot.
 * Switched from the stale game_engine_class fork to the canonical struct game_engine. */
#include "headers/game_engine.h"
void game_engine_dispose_from_old_map(void)
{
    if ( game_engine && game_engine->dispose_from_old_map )
        game_engine->dispose_from_old_map();
}
