#pragma once
#include <stdint.h>
#include "vidPASS_OBJ_MTL.h"
// ws-engine vid: per-object render-pass state. DB-verified layout (types_members vidPASS_OBJ)
// -- size 156 (`types`.size), members coordSpace@0/1, skinType@1/1, billboard@2/1, z_bias@4,
// z_bias_ss@8, scale@12, decal@16/1, geom@20, mtl@24/132. Only the `mtl` sub-object (the flattened
// vidPASS_OBJ::MTL) is fully modelled here; the scalar/enum fields ahead of it are named at their
// DB offsets. Natural alignment reproduces every DB offset, so no explicit padding members are
// declared (that also keeps all nine members visible to the header_members gate, which stops
// comparing at a header's first FILLER member).
//
// Single canonical body as of the odr_dup drain: ws/vid/vidOBJ_SPLIT.h used to carry a second,
// differing vidPASS_OBJ (same nine DB members and the same 156 bytes, but with explicit _pad
// members and with `mtl` typed as a locally-nested `struct MTL` rather than vidPASS_OBJ_MTL).
// That is `error: redefinition` in the header_layout probe TU and a live layout hazard, since
// which body a TU saw depended on include order. This one survives because the corpus's single
// model of the DB type vidPASS_OBJ::MTL is vidPASS_OBJ_MTL (whose per-T ctors are reversed in
// src/vidPASS_OBJ_MTL*.c, and which scrSCORCH_MANAGER__RemoveAll.cpp binds by name); keeping the
// nested spelling would have left the corpus with two competing models of one 132-byte DB type.
//
// Merged in from that deleted body (its only content this one lacked):
//  * the three nested enums below, with their DB values -- and its DEVIATION note, which is
//    load-bearing: the explicit `: unsigned char` underlying type is what keeps them 1 byte each.
//    A plain C++ enum is 4 bytes under MSVC, which made vidPASS_OBJ 168 instead of 156 (shifting
//    mtl to 0x24) and vidOBJ_SPLIT 296 instead of 284.
//  * NOT merged, because it lives in vidPASS_OBJ_MTL.h and that header is outside this drain's
//    slice: the deleted body additionally NAMED the 1-byte enum fields that vidPASS_OBJ_MTL.h
//    still models as unnamed padding -- LM::streams (`lm._padD`, DB LM::STREAMS, 3 bytes at
//    lm+0x0D) and, in each of LAYER0/1/2 (`_pad18[8]`), mask@+0x18 (LAYERn::MASK),
//    vcMaskComp@+0x19 (LAYERn::VCMASKCOMP), blend@+0x1A (LAYERn::BLEND) and streams@+0x1B
//    (LAYERn::STREAMS, 3 bytes), with 2 bytes of DB-verified tail padding at +0x1E.

typedef struct vidPASS_OBJ {
    // DB-verified nested enums (types_enum_values vidPASS_OBJ::*), 1-byte storage each.
    // DEVIATION: the explicit `: unsigned char` underlying type is load-bearing -- see above.
    enum COORDSPACE : unsigned char { COORDSPACE_LOCAL = 0, COORDSPACE_WORLD = 1, COORDSPACE_SCREEN = 2, COORDSPACE_PROJ = 3 };
    enum SKINTYPE   : unsigned char { SKINTYPE_NONE = 0, SKINTYPE_DUAL_QUAT = 1, SKINTYPE_COMPOUND = 2 };
    enum BILLBOARD  : unsigned char { BILLBOARD_NONE = 0, BILLBOARD_SIMPLE = 1, BILLBOARD_AXIS_Y = 2 };

    COORDSPACE       coordSpace; // 0x00 (1 byte)
    SKINTYPE         skinType;   // 0x01 (1 byte)
    BILLBOARD        billboard;  // 0x02 (1 byte)
    float            z_bias;     // 0x04
    float            z_bias_ss;  // 0x08
    float            scale;      // 0x0C
    bool             decal;      // 0x10
    unsigned int     geom;       // 0x14
    vidPASS_OBJ_MTL  mtl;        // 0x18 (132) flattened vidPASS_OBJ::MTL
} vidPASS_OBJ;                    // 156 bytes
