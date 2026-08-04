#pragma once
/* Win32 file-enumeration boundary (platform APIs).
   NOTE: the DB (Xbox 360 build) stores dwHighDateTime FIRST in _FILETIME —
   kept verbatim (types_members _FILETIME / FILETIME agree). */

typedef struct _FILETIME
{
    unsigned int dwHighDateTime; /* 0 */
    unsigned int dwLowDateTime;  /* 4 */
} _FILETIME;
typedef _FILETIME FILETIME;

/* types_members _WIN32_FIND_DATAA (size 320) */
typedef struct _WIN32_FIND_DATAA
{
    unsigned int dwFileAttributes; /* 0 */
    _FILETIME ftCreationTime;      /* 4 */
    _FILETIME ftLastAccessTime;    /* 12 */
    _FILETIME ftLastWriteTime;     /* 20 */
    unsigned int nFileSizeHigh;    /* 28 */
    unsigned int nFileSizeLow;     /* 32 */
    unsigned int dwReserved0;      /* 36 */
    unsigned int dwReserved1;      /* 40 */
    char cFileName[260];           /* 44 */
    char cAlternateFileName[14];   /* 304 */
} _WIN32_FIND_DATAA;

unsigned int GetFileAttributesA(const char *fileName);
void *FindFirstFileA(const char *fileName, _WIN32_FIND_DATAA *findData);
int FindNextFileA(void *findHandle, _WIN32_FIND_DATAA *findData);
int CloseHandle(void *object);
