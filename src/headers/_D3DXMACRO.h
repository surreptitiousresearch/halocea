#pragma once

/* D3DX preprocessor macro definition passed to effect/shader compilation.
   DB-verified: types_members(_D3DXMACRO). */
typedef struct _D3DXMACRO
{
    const char *Name;       /* 0x00 */
    const char *Definition; /* 0x04 */
} _D3DXMACRO; /* 8 bytes */
