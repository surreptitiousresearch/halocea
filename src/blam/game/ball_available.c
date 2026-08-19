/* ball_available @0x83816438 — true if any oddball spawn slot is both untimed-out and unowned.
 *
 * DEVIATION: the decompiler renders this as a loop-continuation condition on `*(i-16)` (a raw pointer 64
 * bytes before `current_ball_owner`); disasm_range(0x83816438,0x838164A4) confirms `-0x40(r11)` is
 * `ball_spawn_timer[k]` (0xC4-0x84 = 0x40, matching oddball_globals.h's field gap) walked in lockstep with
 * `current_ball_owner[k]`. Rewritten as the direct positive condition it actually tests. */

#include <stdint.h>
#include "headers/game_variant.h"
#include "headers/oddball_globals.h"

extern game_variant *game_engine_get_variant(void);

uint8_t ball_available(void)
{
    int ball_spawn_count = game_engine_get_variant()->game_engine_variant.oddball.ball_spawn_count;

    for ( int i = 0; i < ball_spawn_count; ++i )
    {
        if ( oddball_globals.ball_spawn_timer[i] == 0 && oddball_globals.current_ball_owner[i] == -1 )
            return 1;
    }
    return 0;
}
