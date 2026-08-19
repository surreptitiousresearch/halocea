/* cinematic_in_progress @ 0x83691878 — lbz 9(cinematic_globals); callers clrlwi r3,24 -> uint8_t */
#include <stdint.h>
#include "headers/cinematic_globals.h"
uint8_t cinematic_in_progress(void)
{
    return cinematic_globals->cinematic_in_progress;
}
