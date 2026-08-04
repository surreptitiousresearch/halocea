#pragma once
#include "vidOBJ_SPLIT_OCC_RES.h"
#include "../txm/txmTEXTURE_PTR.h"
#include "../../d3dOBJ_PASS_LIST.h"
#include "../ds/dsVECTOR.h"
#include "../ds/dsTSTRING.h"
#include "vidMTL.h"
// ws-engine vid: one draw-batch split of a render object, grouped by material/pass state.
// DB-verified layout (types_members vidOBJ_SPLIT): occRes@0 (vidOBJ_SPLIT_OCC_RES, 16B),
// passList@16 (d3dOBJ_PASS_LIST, 88B), objDesc@104 (vidPASS_OBJ, 156B), mtlList@260
// (dsVECTOR<vidMTL,8>, 20B), curMtl@280 (char) — size 281 (rounds up with tail padding).
//
// CAVEAT: passList (d3dOBJ_PASS_LIST) is a large, deeply-nested D3D pass-description type
// outside this batch's closure; per the established vid* boundary convention (large
// self-contained video-driver classes reached through a handful of methods) it is kept as a
// DB-size-accurate opaque byte blob. objDesc (vidPASS_OBJ, DB-verified types_members
// vidPASS_OBJ) IS given a thin typed shell below because dsVECTOR<vidOBJ_SPLIT,8>::
// ShrinkResize/Resize call vidPASS_OBJ::MTL's destructor directly on the sub-object at
// objDesc.mtl (offset 24 within vidPASS_OBJ) — MTL itself (a D3D material-state block) stays
// an opaque DB-size-accurate blob with only its destructor exposed as a boundary.

typedef struct vidPASS_OBJ {
    // vidPASS_OBJ::MTL — D3D per-pass material/texture-stage state (132 bytes; DB-verified,
    // types_members vidPASS_OBJ::MTL and nested BASE/LM/LAYER0/1/2 — the fully-typed flattened
    // spelling lives in vidPASS_OBJ_MTL.h). Only its destructor is called by this batch
    // (releases held D3D resource references).
    struct MTL {
        struct BASE {
            int             tint;            /* 0x00 */
            txmTEXTURE_PTR  tex;             /* 0x04 */
            float           texDensityU;     /* 0x08 */
            float           texDensityV;     /* 0x0C */
            unsigned short  streams;         /* 0x10 BASE::STREAMS (2B) */
            unsigned char   _pad12[2];       /* 0x12 db-verified tail padding */
        } base;                              /* 0x00 */
        struct LM {
            txmTEXTURE_PTR  texDif;          /* 0x00 */
            txmTEXTURE_PTR  texDir;          /* 0x04 */
            float           maxIntensity;    /* 0x08 */
            unsigned char   source;          /* 0x0C LM::SOURCE (1B) */
            unsigned char   streams[3];      /* 0x0D LM::STREAMS (3B) */
        } lm;                                /* 0x14 */
        struct LAYER0 {
            int             tint;            /* 0x00 */
            float           texDensityU;     /* 0x04 */
            float           texDensityV;     /* 0x08 */
            txmTEXTURE_PTR  texDiff;         /* 0x0C */
            txmTEXTURE_PTR  texNM;           /* 0x10 */
            txmTEXTURE_PTR  texSpec;         /* 0x14 */
            unsigned char   mask;            /* 0x18 LAYER0::MASK (1B) */
            unsigned char   vcMaskComp;      /* 0x19 LAYER0::VCMASKCOMP (1B) */
            unsigned char   blend;           /* 0x1A LAYER0::BLEND (1B) */
            unsigned char   streams[3];      /* 0x1B LAYER0::STREAMS (3B) */
            unsigned char   _pad1E[2];       /* 0x1E db-verified tail padding */
        } layer0;                            /* 0x24 */
        struct LAYER1 {
            int             tint;            /* 0x00 */
            float           texDensityU;     /* 0x04 */
            float           texDensityV;     /* 0x08 */
            txmTEXTURE_PTR  texDiff;         /* 0x0C */
            txmTEXTURE_PTR  texNM;           /* 0x10 */
            txmTEXTURE_PTR  texSpec;         /* 0x14 */
            unsigned char   mask;            /* 0x18 LAYER1::MASK (1B) */
            unsigned char   vcMaskComp;      /* 0x19 LAYER1::VCMASKCOMP (1B) */
            unsigned char   blend;           /* 0x1A LAYER1::BLEND (1B) */
            unsigned char   streams[3];      /* 0x1B LAYER1::STREAMS (3B) */
            unsigned char   _pad1E[2];       /* 0x1E db-verified tail padding */
        } layer1;                            /* 0x44 */
        struct LAYER2 {
            int             tint;            /* 0x00 */
            float           texDensityU;     /* 0x04 */
            float           texDensityV;     /* 0x08 */
            txmTEXTURE_PTR  texDiff;         /* 0x0C */
            txmTEXTURE_PTR  texNM;           /* 0x10 */
            txmTEXTURE_PTR  texSpec;         /* 0x14 */
            unsigned char   mask;            /* 0x18 LAYER2::MASK (1B) */
            unsigned char   vcMaskComp;      /* 0x19 LAYER2::VCMASKCOMP (1B) */
            unsigned char   blend;           /* 0x1A LAYER2::BLEND (1B) */
            unsigned char   streams[3];      /* 0x1B LAYER2::STREAMS (3B) */
            unsigned char   _pad1E[2];       /* 0x1E db-verified tail padding */
        } layer2;                            /* 0x64 */
        ~MTL(); // boundary
    };

    // DB-verified nested enums (types_enum_values vidPASS_OBJ::*), 1-byte storage each.
    enum COORDSPACE { COORDSPACE_LOCAL = 0, COORDSPACE_WORLD = 1, COORDSPACE_SCREEN = 2, COORDSPACE_PROJ = 3 };
    enum SKINTYPE   { SKINTYPE_NONE = 0, SKINTYPE_DUAL_QUAT = 1, SKINTYPE_COMPOUND = 2 };
    enum BILLBOARD  { BILLBOARD_NONE = 0, BILLBOARD_SIMPLE = 1, BILLBOARD_AXIS_Y = 2 };

    COORDSPACE    coordSpace;  // 0x00 (1 byte)
    SKINTYPE      skinType;    // 0x01 (1 byte)
    BILLBOARD     billboard;   // 0x02 (1 byte)
    unsigned char _pad3;       // 0x03
    float         z_bias;      // 0x04
    float         z_bias_ss;   // 0x08
    float         scale;       // 0x0C
    bool          decal;       // 0x10
    unsigned char _pad17[3];   // 0x11
    unsigned int  geom;        // 0x14
    MTL           mtl;         // 0x18
} vidPASS_OBJ;

typedef struct vidOBJ_SPLIT {
    vidOBJ_SPLIT_OCC_RES  occRes;          // 0x000
    d3dOBJ_PASS_LIST      passList;        // 0x010 (see ../../d3dOBJ_PASS_LIST.h)
    vidPASS_OBJ           objDesc;         // 0x068
    dsVECTOR<vidMTL, 8>   mtlList;         // 0x104 resolved material slots for this split
    char                  curMtl;          // 0x118 index (as a signed byte) of the currently-bound material

    // 0x82C87370 (const) -- linear scan of mtlList for the slot whose name matches `name`;
    // returns its index, or -1 if not found.
    int FindMtl(const dsTSTRING<char> &name) const;

    // Default-construct: boundary (only its call sites are reversed).
    vidOBJ_SPLIT(); // boundary
} vidOBJ_SPLIT;
