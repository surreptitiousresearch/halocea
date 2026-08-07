#pragma once
#include <stdint.h>
#include "../ap/apSTATE_T.h"
#include "../m3d/m3dVTX.h"
#include "objGEOM_SHARED.h"
// ws-engine obj: per-object (unshared) geometry wrapper around an objGEOM_SHARED.
// DB-verified layout (types_members objGEOM_UNSHARED) — size 72 (0x48).
// DEVIATION: this comment previously said "size 71 (0x47)". The DB `types` row for
// objGEOM_UNSHARED is 72; the last member is transpRendPriority@0x46 (1 byte), so byte 0x47 is
// trailing padding. sizeof() is 72 either way (struct alignment is >= 4), so no member is added —
// only the stated size is corrected. (The duplicate body in ws/anim/objOBJ_boundary.h, deleted in
// the odr_dup drain, was the copy that had this right, via an explicit `_pad47[1]` tail member.)

struct objOBJ;       // boundary — pointer only
struct rendORD;      // boundary — pointer only
struct objMODIFIER;  // boundary — pointer only

#ifndef objGEOM_UNSHARED_DEFINED
#define objGEOM_UNSHARED_DEFINED
typedef struct objGEOM_UNSHARED {
    apSTATE_T<int64_t> stateDyn;             // 0x00
    objOBJ            *pSharingHostObj;       // 0x08
    objGEOM_SHARED    *pSharedGeom;           // 0x0C
    int                sharingOfsVtx;         // 0x10
    int                sharingOfsFace;        // 0x14
    rendORD           *pRendData;             // 0x18
    unsigned int       color;                 // 0x1C
    unsigned int       color_mp;              // 0x20
    m3dVTX             texOffset[2];          // 0x24
    objMODIFIER      **modList;               // 0x34
    float              transp;                // 0x38
    float              lodTransp;             // 0x3C
    int16_t            nModifiers;            // 0x40
    uint8_t    alphaKillValue;        // 0x42
    uint8_t    zBiasValue;            // 0x43
    char               renderPassId;          // 0x44
    uint8_t    blendRend;             // 0x45
    uint8_t    transpRendPriority;    // 0x46
} objGEOM_UNSHARED;
#endif
