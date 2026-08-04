#include <stdint.h>
/* hs_inspect_short_integer @0x8368C9D8 — format a short (16-bit) HaloScript value into a text buffer. The
 * value lives in the high word of the 32-bit value word (PPC big-endian). Installed in hs_type_inspectors[7]. */

extern int sprintf_0(char *string, const char *format, ...);

void hs_inspect_short_integer(int16_t type, int value, char *buffer)
{
    sprintf_0(buffer, "%d", (short)(value >> 16));   /* SHIWORD(value) */
}
