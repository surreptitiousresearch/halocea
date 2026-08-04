#pragma once
/* game_globals (tag definition) — the resolved 'matg' game globals tag, 0x1AC/428 bytes.
 * The authoritative definition lives in game_globals.h as the bare TAG `struct game_globals`
 * (bare tag because the ordinary-namespace name `game_globals` is the runtime pointer variable).
 * This header used to duplicate the layout with a colliding `game_globals` typedef; it is now a
 * shim providing a non-colliding typedef alias for consumers that spell the type without
 * the struct keyword. */

#include "game_globals.h"

typedef struct game_globals game_globals_definition; /* 0x1AC (428 bytes) */
