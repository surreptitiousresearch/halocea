/* game_engine_game_ending @0x83746220 — dispatches to the active game engine's game_ending callback, if any.
 *
 * DEVIATION: the decompiler renders this as returning the callback's result, but every concrete
 * game_ending implementation (ctf_engine_game_ending.c, king_/oddball_/race_/slayer_/stub_engine_game_ending.c)
 * is a bare `blr` no-op — the callback is genuinely `void (*)(void)` (matches game_engine.h's field type).
 * The DB's non-void return for this wrapper reflects an uninitialized/unused r3, not a real value; returns 0. */

#include "headers/game_engine.h"


int game_engine_game_ending(void)
{
    if (game_engine && game_engine->game_ending)
        game_engine->game_ending();
    return 0;
}
