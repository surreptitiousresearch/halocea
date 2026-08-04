/* user_mode_safe_fopen @0x83766618 — open a file by copying the path into a shared bounded buffer (max 0x104)
 * before calling fopen, guarding against an over-long path.
 *
 * Boundary: _snprintf_0 / fopen are CRT externs. */

#include <stdio.h>
#include "headers/blam_data_globals.h"

extern int _snprintf_0(char *buffer, size_t count, const char *format, ...);

FILE *user_mode_safe_fopen(const char *filename, const char *how)
{
    result_0[0] = 0;
    _snprintf_0(result_0, 0x104u, "%s", filename);
    return fopen(result_0, how);
}
