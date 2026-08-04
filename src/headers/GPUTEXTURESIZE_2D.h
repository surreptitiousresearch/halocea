#pragma once

/* Xenos GPU packed 2D texture size (external GPU/D3D boundary type). DB: struct, size 4. */
typedef struct GPUTEXTURESIZE_2D
{
    unsigned __int32 Width : 13;
    unsigned __int32 Height : 13;
} GPUTEXTURESIZE_2D; /* 4 bytes */
