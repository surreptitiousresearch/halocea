#pragma once

/* Xenos GPU packed stacked-texture size (external GPU/D3D boundary type). DB: struct, size 4. */
typedef struct GPUTEXTURESIZE_STACK
{
    unsigned __int32 Width : 13;
    unsigned __int32 Height : 13;
    unsigned __int32 Depth : 6;
} GPUTEXTURESIZE_STACK; /* 4 bytes */
