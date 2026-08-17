/* system_clocks_to_microseconds @0x83762D00 */
#include "headers/blam_data_globals.h"

/* DEVIATION: decompiler output garbled; reconstructed from disasm (mulld/divdu pattern) */
unsigned long long system_clocks_to_microseconds(unsigned long long clocks)
{
    return clocks * 1000000ull / (unsigned long long)s_ClockFrequency.QuadPart;
}
