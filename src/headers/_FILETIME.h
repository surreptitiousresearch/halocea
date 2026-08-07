#ifndef _FILETIME_H
#define _FILETIME_H

#include <stdint.h>

/* NOTE: the DB (Xbox 360 build) stores dwHighDateTime FIRST — kept verbatim, types_members
 * _FILETIME and FILETIME agree. (Merged 2026-08-07 from the duplicate body that used to live in
 * src/headers/havok/win32_file_boundary.h, which now includes this header.) */
typedef struct _FILETIME
{
    uint32_t dwHighDateTime;
    uint32_t dwLowDateTime;
} _FILETIME;

typedef struct _FILETIME FILETIME; /* Win32 alias */

#endif /* _FILETIME_H */
