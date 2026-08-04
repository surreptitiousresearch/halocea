#pragma once

/* ws-engine Xenon render-target slot. DB-verified (types_members XENON_RT). */
#include "XENON_RT_DESC.h"

typedef struct D3DSurface D3DSurface;

typedef struct XENON_RT
{
    XENON_RT_DESC desc; /* 0x00 */
    D3DSurface *surf;   /* 0x18 */
} XENON_RT; /* 28 bytes */
