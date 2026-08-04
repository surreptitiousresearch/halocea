#pragma once
/* achievements_info @ 1024 (0x400) bytes — reserved achievements state block. */

typedef struct achievements_info
{
    int reserved[256]; /* 0x000 */
} achievements_info;   /* 1024 bytes */
