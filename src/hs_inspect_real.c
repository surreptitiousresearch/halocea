#include <stdint.h>
/* hs_inspect_real @0x8368C998 — format a real (float) HaloScript value into a text buffer. The 32-bit value
 * word is reinterpreted as an IEEE float. Installed in hs_type_inspectors[6]. */

extern int sprintf_0(char *string, const char *format, ...);

void hs_inspect_real(int16_t type, int value, char *buffer)
{
    sprintf_0(buffer, "%f", *(float *)&value);
}
