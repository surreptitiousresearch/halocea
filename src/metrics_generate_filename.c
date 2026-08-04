/* metrics_generate_filename @0x83818D04 — build the path for a metrics dump file:
 * "message metrics\<timestamp> <base>". */

#include "headers/metrics_globals.h"

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);

void metrics_generate_filename(const char *base, char *destination, unsigned int max_size)
{
    /* backslash escaped 2026-07-30 (DB string 0x821299F4 = "%s\%s %s"; "\%" dropped the '\') */
    _snprintf_0(destination, max_size, "%s\\%s %s", "message metrics", metrics_globals.metrics_timestamp, base);
}
