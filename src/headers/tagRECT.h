#pragma once

/* Win32 RECT (declared here to avoid a Win32 header dependency). DB-verified. */
typedef struct tagRECT {
    int left;   /* 0x00 */
    int top;    /* 0x04 */
    int right;  /* 0x08 */
    int bottom; /* 0x0C */
} tagRECT; /* 16 bytes */
