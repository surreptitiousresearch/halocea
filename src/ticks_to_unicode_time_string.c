/* ticks_to_unicode_time_string @0x83749CF8 — wide-string counterpart of ticks_to_time_string.c: formats a
 * tick count (30 ticks/sec) as "H:MM".
 *
 * DEVIATION: the decompiler dropped the integer value argument from both usnprintf(..., L"%d") calls,
 * rendering them as if called with no value. Disasm shows the value register (r6) is actually set by the
 * `divw.` (hours) and an earlier unconditional `mr r6,r30` (minutes) — a dual-purpose instruction (also
 * setting the branch-condition flags) that the decompiler failed to recognize as a live call argument.
 * Restored to pass the value explicitly, matching ticks_to_time_string.c's identical structure. */

#include <stdint.h>
#include <stddef.h>   /* wchar_t */

extern void usnprintf(wchar_t *string, unsigned int size, const wchar_t *format, ...);

void ticks_to_unicode_time_string(int ticks, unsigned int count, uint16_t *buffer)
{
    int minutes = ticks / 30 % 60;
    int hours = ticks / 30 / 60;
    uint16_t hours_string[64];
    uint16_t minutes_string[64];

    if ( hours )
        usnprintf(hours_string, 0x40u, L"%d", hours);
    else
        usnprintf(hours_string, 0x40u, L" ");

    if ( minutes > 9 )
        usnprintf(minutes_string, 0x40u, L"%d", minutes);
    else
        usnprintf(minutes_string, 0x40u, L"0%d", minutes);

    usnprintf(buffer, count, L"%s:%s", hours_string, minutes_string);
}
