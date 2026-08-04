/* cinematic_skip_start @0x83690F78 — mark that the player has begun skipping the current cinematic. */

#include "headers/cinematic_globals.h"

void cinematic_skip_start(void)
{
    cinematic_globals->cinematic_skip_in_progress = 1;
}
