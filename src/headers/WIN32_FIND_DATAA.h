#pragma once
/* WIN32_FIND_DATAA — the Win32 directory-entry record filled by FindFirstFileA/FindNextFileA.
 * Standard platform layout. Embeds three _FILETIME members (DB member type is _FILETIME). */

#include "_FILETIME.h"

typedef struct _WIN32_FIND_DATAA
{
    unsigned int dwFileAttributes;       /* 0x00 */
    _FILETIME    ftCreationTime;         /* 0x04 */
    _FILETIME    ftLastAccessTime;       /* 0x0C */
    _FILETIME    ftLastWriteTime;        /* 0x14 */
    unsigned int nFileSizeHigh;          /* 0x1C */
    unsigned int nFileSizeLow;           /* 0x20 */
    unsigned int dwReserved0;            /* 0x24 */
    unsigned int dwReserved1;            /* 0x28 */
    char         cFileName[260];         /* 0x2C */
    char         cAlternateFileName[14]; /* 0x130 */
} WIN32_FIND_DATAA, _WIN32_FIND_DATAA; /* 320 bytes; _WIN32_FIND_DATAA alias matches DB type name */
