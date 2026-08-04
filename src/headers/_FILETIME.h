#ifndef _FILETIME_H
#define _FILETIME_H

#include <stdint.h>

typedef struct _FILETIME
{
    uint32_t dwHighDateTime;
    uint32_t dwLowDateTime;
} _FILETIME;

typedef struct _FILETIME FILETIME; /* Win32 alias */

#endif /* _FILETIME_H */
