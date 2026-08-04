#pragma once

/* Direct3D9 pixel-shader 2.0 capability sub-struct (external library boundary type).
   DB-verified 20-byte layout. */
typedef struct _D3DPSHADERCAPS2_0
{
    unsigned int Caps;                    /* 0x00 */
    int          DynamicFlowControlDepth; /* 0x04 */
    int          NumTemps;                /* 0x08 */
    int          StaticFlowControlDepth;  /* 0x0C */
    int          NumInstructionSlots;     /* 0x10 */
} _D3DPSHADERCAPS2_0; /* 20 bytes */
