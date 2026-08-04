#pragma once
// vidMODE -- ws-engine video-mode descriptor. Layout DB-verified against types_members
// (vidMODE, sizeof=0x28). External boundary type in origin, but fully modeled here at its
// authoritative offsets. XREF: d3dMODE.baseclass_0.
#include "ws/ap/apSTATE_T.h"

typedef struct vidMODE
{
    __int64                  vmState;       /* 0x00 */
    apSTATE_T<unsigned long>  state;        /* 0x08 */
    int                       freq;         /* 0x0C */
    int                       sx;           /* 0x10 */
    int                       sy;           /* 0x14 */
    int                       bpp;          /* 0x18 */
    float                     pixel_aspect; /* 0x1C */
    int                       sxRT;         /* 0x20 */
    int                       syRT;         /* 0x24 */
} vidMODE; /* 40 bytes */
