/* game_engine_game_ending @0x83746220 — dispatches to the active game engine's game_ending callback, if any.
 *
 * DEVIATION: the decompiler renders this as returning the callback's result, but every concrete
 * game_ending implementation (ctf_engine_game_ending.c, king_/oddball_/race_/slayer_/stub_engine_game_ending.c)
 * is a bare `blr` no-op — the callback is genuinely `void (*)(void)` (matches game_engine.h's field type).
 * The DB's non-void return for this wrapper reflects an uninitialized/unused r3, not a real value.
 *
 * DEVIATION (#119): the wrapper is `void`, and the earlier `return 0;` was a value the binary never
 * materialises — 0x83746220-0x83746248 is eleven instructions with no `li r3, N` anywhere: both guard
 * exits are bare `beqlr` (@0x8374622C, @0x83746238) and the dispatch is a tail `bctr` @0x83746244
 * through game_engine+0x1C. The function has no xrefs at all, so no caller consumes r3. */

#include "headers/game_engine.h"


void game_engine_game_ending(void)
{
    if (game_engine && game_engine->game_ending)
        game_engine->game_ending();
}
