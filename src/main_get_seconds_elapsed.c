/* main_get_seconds_elapsed @0x83688A48 — return the wall-clock seconds elapsed during the last frame.
 *
 * Deviation: the decompiler's float/double register juggling is PPC ABI noise; this is a plain field read. */

#include "headers/main_globals.h"

float main_get_seconds_elapsed(void)
{
    return main_globals.seconds_elapsed;
}
