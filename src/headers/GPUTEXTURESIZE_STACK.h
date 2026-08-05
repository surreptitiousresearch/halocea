#pragma once
#include <stdint.h>

/* Xenos GPU packed stacked-texture size (external GPU/D3D boundary type). DB: struct, size 4. */
typedef struct GPUTEXTURESIZE_STACK
{
    uint32_t Width : 13;
    uint32_t Height : 13;
    uint32_t Depth : 6;
} GPUTEXTURESIZE_STACK; /* 4 bytes */
