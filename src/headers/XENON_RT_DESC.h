#pragma once

/* ws-engine Xenon render-target descriptor. DB-verified (types_members XENON_RT_DESC;
 * enum values from types_enum_values XENON_RT_DESC::OVERLAP — nested enum spelled at
 * file scope, namespace qualification only, same type). */
#include "_D3DMULTISAMPLE_TYPE.h"
#include "_D3DFORMAT.h"

typedef enum OVERLAP
{
    OVERLAP_BB = 0,
    OVERLAP_ZB = 1,
    OVERLAP_NO = 2,
} OVERLAP; /* XENON_RT_DESC::OVERLAP, 4 bytes */

typedef struct XENON_RT_DESC
{
    OVERLAP overlapping;             /* 0x00 — XENON_RT_DESC::OVERLAP */
    _D3DMULTISAMPLE_TYPE multisample; /* 0x04 */
    unsigned int width;              /* 0x08 */
    unsigned int height;             /* 0x0C */
    _D3DFORMAT format;               /* 0x10 */
    int splitIndex;                  /* 0x14 */
} XENON_RT_DESC; /* 24 bytes */
