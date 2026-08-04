#pragma once

/* Direct3D9 vertex-shader 2.0 capability sub-struct (external library boundary type).
   DB-verified 16-byte layout. */
typedef struct _D3DVSHADERCAPS2_0
{
    unsigned int Caps;                    /* 0x00 */
    int          DynamicFlowControlDepth; /* 0x04 */
    int          NumTemps;                /* 0x08 */
    int          StaticFlowControlDepth;  /* 0x0C */
} _D3DVSHADERCAPS2_0; /* 16 bytes */
