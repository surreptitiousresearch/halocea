#pragma once
/* _GUID — standard 16-byte globally-unique identifier (Win32/Xbox layout). */

typedef struct _GUID
{
    unsigned int   Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} _GUID;
