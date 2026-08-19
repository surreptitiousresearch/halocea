/* main_get_last_time_clocks @0x8368AE80 — return the clock counter captured at the start of the last frame.
 *
 * Deviation: the field is __int64; the function returns only the low 32 bits (decompiler-truncated). */

#include "headers/main_globals.h"

int main_get_last_time_clocks(void)
{
    return (int)main_globals.last_time_clocks;
}
