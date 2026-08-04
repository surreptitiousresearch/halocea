#pragma once

/* Xbox 360 D3D query-type enum (external library boundary type). DB-verified
 * (types_enum_values _D3DQUERYTYPE). */
typedef enum _D3DQUERYTYPE
{
    D3DQUERYTYPE_VCACHE          = 4,
    D3DQUERYTYPE_RESOURCEMANAGER = 5,
    D3DQUERYTYPE_VERTEXSTATS     = 6,
    D3DQUERYTYPE_EVENT           = 8,
    D3DQUERYTYPE_OCCLUSION       = 9,
    D3DQUERYTYPE_SCREENEXTENT    = 10,
} _D3DQUERYTYPE; /* 4 bytes */
