#pragma once

#include "_D3DFORMAT.h"
#include "_D3DRESOURCETYPE.h"
#include "_D3DMULTISAMPLE_TYPE.h"

/* Xbox 360 D3D surface descriptor (external library boundary type). DB-verified. */
typedef struct D3DSURFACE_DESC {
    _D3DFORMAT           Format;             /* 0x00 */
    _D3DRESOURCETYPE     Type;               /* 0x04 */
    unsigned int         Usage;              /* 0x08 */
    unsigned int         Pool;               /* 0x0C */
    _D3DMULTISAMPLE_TYPE MultiSampleType;    /* 0x10 */
    unsigned int MultiSampleQuality; /* 0x14 */
    unsigned int Width;              /* 0x18 */
    unsigned int Height;             /* 0x1C */
} D3DSURFACE_DESC; /* 32 bytes */
