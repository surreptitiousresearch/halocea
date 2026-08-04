#pragma once

#include "pctHEADER.h"          /* pctHEADER (embedded by value) */
#include "ws/ds/dsVECTOR.h"     /* dsVECTOR<pctPIXEL,8> (pointee) */

struct pctPIXEL;                 /* fwd: defined in ws/gs/gsRENDER_SYSTEM_boundary.h */

/* Picture/texture-format subsystem (pct*) boundary — not decompiled. Layout
   DB-verified via types_members pctPICTURE (36 bytes). */
typedef struct pctPICTURE
{
    pctHEADER              hdr;      /* 0x00 */
    dsVECTOR<pctPIXEL, 8> *pPalette; /* 0x1C */
    void                 **pFaces;   /* 0x20 */
} pctPICTURE;                        /* 36 bytes */
