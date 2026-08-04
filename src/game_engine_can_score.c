/* game_engine_can_score @0x83747770 — true when no game engine is loaded (free play) or the loaded game
 * engine is in the active (scoring) mode. */

#include "headers/game_engine_globals.h"
/* DEVIATION: the decompiler modeled game_engine_globals as {mode@0} and guessed active==1; the DB-verified
 * layout (game_engine_globals.h) has mode@0x10 and game_engine_mode_active==0 — disasm computes mode==0
 * via cntlzw/extrwi. Reusing the canonical header corrects both the offset and the compared value. */

#include <stdint.h>

extern void *game_engine;

uint8_t game_engine_can_score(void)
{
    if ( game_engine )
        return game_engine_globals.mode == game_engine_mode_active;
    return 1;
}
