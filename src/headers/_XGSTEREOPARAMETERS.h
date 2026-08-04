#pragma once

/* Xbox 360 XGraphics stereo parameters (external library boundary type).
 * DB-verified (types_members _XGSTEREOPARAMETERS). */
#include "_XGSTEREOREGION.h"

typedef struct _XGSTEREOPARAMETERS
{
    unsigned int FrontBufferWidth;  /* 0x00 */
    unsigned int FrontBufferHeight; /* 0x04 */
    unsigned int EyeBufferWidth;    /* 0x08 */
    unsigned int EyeBufferHeight;   /* 0x0C */
    _XGSTEREOREGION LeftEye;        /* 0x10 */
    _XGSTEREOREGION BlankRegion;    /* 0x5C */
    _XGSTEREOREGION RightEye;       /* 0xA8 */
    unsigned int Reserved;          /* 0xF4 */
} _XGSTEREOPARAMETERS; /* 248 bytes */
