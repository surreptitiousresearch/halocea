/* vibrate_player_set_scale @0x83739188 */
#include "headers/vibrate_global_data.h"

void vibrate_player_set_scale(float scale)
{
    /* attested float param: stfs f1 directly, no frsp — no double->float conversion exists */
    vibrate_globals->scripted_vibrate.scale = scale;
}
