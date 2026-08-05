#pragma once
#include <stdint.h>

/* Xenos GPU packed 2D texture size (external GPU/D3D boundary type). DB: struct, size 4. */
typedef struct GPUTEXTURESIZE_2D
{
    uint32_t Width : 13;
    uint32_t Height : 13;
} GPUTEXTURESIZE_2D; /* 4 bytes */
