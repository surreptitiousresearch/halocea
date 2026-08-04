#pragma once
/* game_globals (tag) — the scenario's globals tag, pointed to by global_game_globals. Distinct from the
 * runtime struct_game_globals. The authoritative 428-byte layout lives in game_globals.h as the bare
 * TAG `struct game_globals` (bare tag because the ordinary-namespace name `game_globals` is the runtime
 * pointer variable). This header previously carried its own copy of the layout under the tag
 * game_globals_tag; it is now a typedef alias of the single definition. */

#include "game_globals.h"

typedef struct game_globals game_globals_tag; /* 0x1AC (428 bytes) */

extern game_globals_tag *global_game_globals;
