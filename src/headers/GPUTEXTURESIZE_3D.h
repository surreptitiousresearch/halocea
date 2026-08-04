#pragma once

/* Xenos GPU packed 3D texture size (external GPU/D3D boundary type). DB: struct, size 4. */
typedef struct GPUTEXTURESIZE_3D
{
    unsigned __int32 Width : 11;
    unsigned __int32 Height : 11;
    unsigned __int32 Depth : 10;
} GPUTEXTURESIZE_3D; /* 4 bytes */
