/* ticks_to_time_string @0x83749DA0 — format a tick count (30 ticks/sec) as "H:MM" (hours unpadded, minutes
 * zero-padded to 2 digits; hours omitted as a blank field when 0). */

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);

void ticks_to_time_string(int ticks, unsigned int count, char *buffer)
{
    int minutes = ticks / 30 % 60;
    int hours = ticks / 30 / 60;
    char hours_string[64];
    char minutes_string[64];

    if ( hours )
        _snprintf_0(hours_string, 0x40u, "%d", hours);
    else
        _snprintf_0(hours_string, 0x40u, " ");

    if ( minutes > 9 )
        _snprintf_0(minutes_string, 0x40u, "%d", minutes);
    else
        _snprintf_0(minutes_string, 0x40u, "0%d", minutes);

    _snprintf_0(buffer, count, "%s:%s", hours_string, minutes_string);
}
