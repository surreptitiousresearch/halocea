#pragma once
/* thread — a Win32 thread handle wrapper (threads.c) */

typedef struct /* align(4) */ thread
{
    void         *object;  /* 0x0 — Win32 HANDLE */
    unsigned char in_use;  /* 0x4 */
    unsigned char pad[3];  /* 0x5 — align to 8 */
} thread; /* 8 bytes */
