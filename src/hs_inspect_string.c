#include <stdint.h>
/* hs_inspect_string @0x8368CA28 — format a string HaloScript value into a text buffer. The value word is a
 * pointer to the source text. Installed in hs_type_inspectors[9].
 * DEVIATION: `value` is the raw script value word (`int`), not a typed `const char *`. The binary just
 * forwards it (`mr r5, r4`) into sprintf's varargs, and all six inspectors share the one table signature
 * `void (*)(__int16, int, char *)` — the DB applied type of hs_type_inspectors. */

extern int sprintf_0(char *string, const char *format, ...);

void hs_inspect_string(int16_t type, int value, char *buffer)
{
    sprintf_0(buffer, "%s", (const char *)value);
}
