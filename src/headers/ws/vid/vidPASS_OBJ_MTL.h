#pragma once
#include "../txm/txmTEXTURE_PTR.h"
// ws-engine vid: the resolved-material payload of a vidPASS_OBJ render-pass object (per-object
// texture/tint/streams state for the base layer, a lightmap layer, and up to 3 detail layers).
// DB-verified layout (types_members vidPASS_OBJ::MTL and its nested BASE/LM/LAYER0/1/2): base@0
// (20B), lm@20 (16B), layer0@36 (32B), layer1@68 (32B), layer2@100 (32B) -- size 132.
// Only the txmTEXTURE_PTR fields touched by the ctor reconstructions in this batch are named;
// the STREAMS/mask/vcMaskComp/blend enum fields are boundary padding at their DB offsets.
//
// NOTE: the real DB symbol is the nested class `vidPASS_OBJ::MTL` (with nested `MTL::LAYER0/1/2`
// below that); the enclosing `vidPASS_OBJ` class itself is not decompiled anywhere in this corpus,
// so nesting this type inside an otherwise-undefined outer class would add no information.
// Flattened to `vidPASS_OBJ_MTL` here per [[re-source-boundary-conventions]] scoping.

typedef struct vidPASS_OBJ_MTL
{
    struct BASE
    {
        int             tint;         /* 0x00 */
        txmTEXTURE_PTR  tex;          /* 0x04 */
        float           texDensityU;  /* 0x08 */
        float           texDensityV;  /* 0x0C */
        unsigned short  streams;      /* 0x10 BASE::STREAMS -- boundary */
    } base; /* 0x00 */

    struct LM
    {
        txmTEXTURE_PTR  texDif;       /* 0x00 */
        txmTEXTURE_PTR  texDir;       /* 0x04 */
        float           maxIntensity; /* 0x08 */
        unsigned char   source;       /* 0x0C LM::SOURCE -- boundary */
        unsigned char   _padD[16 - 0xD]; /* 0x0D streams -- boundary */
    } lm; /* 0x14 (20) */

    struct LAYER0
    {
        int             tint;        /* 0x00 */
        float           texDensityU; /* 0x04 */
        float           texDensityV; /* 0x08 */
        txmTEXTURE_PTR  texDiff;     /* 0x0C */
        txmTEXTURE_PTR  texNM;       /* 0x10 */
        txmTEXTURE_PTR  texSpec;     /* 0x14 */
        unsigned char   _pad18[32 - 0x18]; /* 0x18 mask/vcMaskComp/blend/streams -- boundary */

        // 0x8272697C -- reversed in vidPASS_OBJ_MTL_LAYER0__LAYER0.c.
        LAYER0();
    } layer0; /* 0x24 (36) */

    struct LAYER1
    {
        int             tint;
        float           texDensityU;
        float           texDensityV;
        txmTEXTURE_PTR  texDiff;
        txmTEXTURE_PTR  texNM;
        txmTEXTURE_PTR  texSpec;
        unsigned char   _pad18[32 - 0x18];

        // 0x8272699C -- reversed in vidPASS_OBJ_MTL_LAYER1__LAYER1.c.
        LAYER1();
    } layer1; /* 0x44 (68) */

    struct LAYER2
    {
        int             tint;
        float           texDensityU;
        float           texDensityV;
        txmTEXTURE_PTR  texDiff;
        txmTEXTURE_PTR  texNM;
        txmTEXTURE_PTR  texSpec;
        unsigned char   _pad18[32 - 0x18];

        // 0x827269BC -- reversed in vidPASS_OBJ_MTL_LAYER2__LAYER2.c.
        LAYER2();
    } layer2; /* 0x64 (100) */

    // 0x82728944 -- reversed in vidPASS_OBJ_MTL__MTL.c. Null-inits every txmTEXTURE_PTR slot
    // across base/lm/layer0/layer1/layer2 (tint/texDensity*/enum fields are left as compiler
    // zero-init via the aggregate's implicit member construction, matching the decompiler's
    // scope -- it only ever writes the .ptr fields).
    vidPASS_OBJ_MTL();
} vidPASS_OBJ_MTL;
