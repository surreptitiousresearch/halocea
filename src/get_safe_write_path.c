#include "headers/blam_data_globals.h"
/* get_safe_write_path @0x837665A0 — copies path into the shared result_0 scratch buffer (via snprintf to
 * guarantee NUL-termination and truncation to its 260-byte capacity) and returns that buffer. */

extern int _snprintf_0(char *buffer, unsigned int size, const char *format, ...);

char * get_safe_write_path(const char *path)
{
    result_0[0] = 0;
    _snprintf_0(result_0, 0x104u, "%s", path);
    return result_0;
}
