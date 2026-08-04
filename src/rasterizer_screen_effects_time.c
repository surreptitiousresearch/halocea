/* rasterizer_screen_effects_time @0x837241C8 — game time in seconds, for driving screen-effect animation.
 *
 * DEVIATION: the decompiler renders the sign-extend-to-int64 idiom as `__SPAIR64__(0x82000000, ...)`;
 * disasm shows a plain extsw/fcfid/frsp int-to-float conversion, reproduced here as a simple cast. */

#include "headers/game_time_constants.h"

extern int game_time_get(void);

float rasterizer_screen_effects_time(void)
{
    return (float)game_time_get() * SECONDS_PER_TICK;
}
