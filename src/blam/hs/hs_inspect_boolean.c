#include <stdint.h>
/* hs_inspect_boolean @0x8368C948 — format a boolean HaloScript value into a text buffer. The value is carried
 * in the high byte of the 32-bit value word (PPC big-endian boolean convention), so a non-zero high byte means
 * "true". Installed in hs_type_inspectors[5]. */

extern int sprintf_0(char *string, const char *format, ...);

void hs_inspect_boolean(int16_t type, int value, char *buffer)
{
    const char *text = ((value >> 24) & 0xFF) ? "true" : "false";   /* HIBYTE(value) */
    sprintf_0(buffer, "%s", text);
}
