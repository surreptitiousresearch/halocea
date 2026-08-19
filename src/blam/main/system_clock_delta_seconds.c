/* system_clock_delta_seconds @0x83762D48 */
#include <stdint.h>
#include "headers/blam_data_globals.h"
/* DEVIATION: decompiler generated spurious weather_particle_system_globals refs; reconstructed from
 * disasm 83762D48: subf(a-b) / ld(s_ClockFrequency) / divd / clrlwi(low32). */

unsigned int system_clock_delta_seconds(int64_t a, int64_t b)
{
    return (unsigned int)((a - b) / s_ClockFrequency.QuadPart);
}
