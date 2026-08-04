#pragma once

typedef struct mutex {
    void *object;             /* 0x00 — Win32 mutex HANDLE */
    char  name[32];           /* 0x04 */
    unsigned char in_use;     /* 0x24 */
    unsigned char pad[3];     /* 0x25 — align(4) to pool stride */
} mutex;                      /* sizeof = 0x28 (40, pool stride) */
