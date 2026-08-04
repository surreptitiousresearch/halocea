/* hcex_is_cine_mode @ 0x83683038
   Returns whether a cinematic is currently playing. */

#include "../headers/cinematic_globals.h"

extern "C" int hcex_is_cine_mode(void)
{
    return cinematic_globals->cinematic_in_progress != 0;
}
