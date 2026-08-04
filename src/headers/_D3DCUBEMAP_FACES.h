#pragma once

/* Xbox 360 D3D cubemap-face enum (external library boundary type). DB-verified
 * (types_enum_values _D3DCUBEMAP_FACES). */
typedef enum _D3DCUBEMAP_FACES
{
    D3DCUBEMAP_FACE_POSITIVE_X   = 0,
    D3DCUBEMAP_FACE_NEGATIVE_X   = 1,
    D3DCUBEMAP_FACE_POSITIVE_Y   = 2,
    D3DCUBEMAP_FACE_NEGATIVE_Y   = 3,
    D3DCUBEMAP_FACE_POSITIVE_Z   = 4,
    D3DCUBEMAP_FACE_NEGATIVE_Z   = 5,
    D3DCUBEMAP_FACE_FORCE_DWORD  = 2147483647,
} _D3DCUBEMAP_FACES; /* 4 bytes */
