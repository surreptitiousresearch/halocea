#pragma once

/* Xbox 360 D3D SDK enum (external library boundary type). DB-verified against
   types_enum_values(_D3DDEVTYPE). */
typedef enum _D3DDEVTYPE
{
    D3DDEVTYPE_HAL            = 1,
    D3DDEVTYPE_COMMAND_BUFFER = 2,
    D3DDEVTYPE_FORCE_DWORD   = 0x7FFFFFFF,
} _D3DDEVTYPE; /* 4 bytes */
