#pragma once

/* Xbox 360 D3D video-scaler parameters (external library boundary type).
 * DB-verified (types_members _D3DVIDEO_SCALER_PARAMETERS). */
#include "_D3DRECT.h"

typedef struct _D3DVIDEO_SCALER_PARAMETERS
{
    _D3DRECT ScalerSourceRect;       /* 0x00 */
    unsigned int ScaledOutputWidth;  /* 0x10 */
    unsigned int ScaledOutputHeight; /* 0x14 */
    unsigned int FilterProfile;      /* 0x18 */
} _D3DVIDEO_SCALER_PARAMETERS; /* 28 bytes */
