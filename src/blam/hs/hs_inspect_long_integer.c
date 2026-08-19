#include <stdint.h>
/* hs_inspect_long_integer @0x8368CA10 — format a long (32-bit) HaloScript value into a text buffer. Installed
 * in hs_type_inspectors[8]. */

extern int sprintf_0(char *string, const char *format, ...);

void hs_inspect_long_integer(int16_t type, int value, char *buffer)
{
    sprintf_0(buffer, "%ld", value);
}
