#pragma once
#include <stdint.h>

/* Xenos GPU packed 3D texture size (external GPU/D3D boundary type). DB: struct, size 4. */
typedef struct GPUTEXTURESIZE_3D
{
    uint32_t Width : 11;
    uint32_t Height : 11;
    uint32_t Depth : 10;
} GPUTEXTURESIZE_3D; /* 4 bytes */
