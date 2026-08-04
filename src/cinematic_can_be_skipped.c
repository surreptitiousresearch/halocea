/* cinematic_can_be_skipped @0x83690F68 — true while a cinematic skip is already in progress. */

#include <stdint.h>
#include "headers/cinematic_globals.h"

/* lbz 0xA(cinematic_globals); all 4 callers clrlwi r3,24 -> uint8_t */
uint8_t cinematic_can_be_skipped(void)
{
    return cinematic_globals->cinematic_skip_in_progress;
}
