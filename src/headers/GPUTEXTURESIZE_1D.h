#pragma once
#include <stdint.h>

/* Xenos GPU packed 1D texture size (external GPU/D3D boundary type). DB: struct, size 4. */
typedef struct GPUTEXTURESIZE_1D
{
    uint32_t Width : 24;
} GPUTEXTURESIZE_1D; /* 4 bytes */
