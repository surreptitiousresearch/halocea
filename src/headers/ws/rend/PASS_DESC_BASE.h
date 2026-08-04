#pragma once
#include "../m3d/m3dCOLOR.h"
#include "../txm/txmTEXTURE_PTR.h"
// ws-engine vid: base render-pass descriptor. DB-verified layout (types_members PASS_DESC_BASE) —
// size 32:
//   rgbSrc@0, alphaSrc@1 (1-byte source-mode enums), colorConst@4 (m3dCOLOR, 16B),
//   blendMode@20, blendModeAlpha@21 (1-byte blend enums), zWrite@22, zTest@23, noCull@24,
//   outputHDR@25, disableGamma@26 (bool), tex@28 (txmTEXTURE_PTR, ref-counting texture handle).
// The 1-byte enums are forward-declared here (their value sets live in the vid subsystem and are
// not needed to construct a descriptor); only BLENDMODE is spelled out elsewhere as a parameter
// type. boundary enums.

typedef struct PASS_DESC_BASE {
    enum RGBSRC : unsigned char;
    enum ALPHASRC : unsigned char;
    enum BLENDMODE : unsigned char;
    enum BLENDMODEALPHA : unsigned char;

    RGBSRC         rgbSrc;          // 0x00
    ALPHASRC       alphaSrc;        // 0x01
    unsigned char  _pad02[2];       // 0x02 alignment gap before colorConst
    m3dCOLOR       colorConst;      // 0x04 (16B) constant modulate color (r,g,b,a in 0..1)
    BLENDMODE      blendMode;       // 0x14
    BLENDMODEALPHA blendModeAlpha;  // 0x15
    bool           zWrite;          // 0x16
    bool           zTest;           // 0x17
    bool           noCull;          // 0x18
    bool           outputHDR;       // 0x19
    bool           disableGamma;    // 0x1A
    unsigned char  _pad1B;          // 0x1B alignment gap before tex
    txmTEXTURE_PTR tex;             // 0x1C ref-counting texture handle
} PASS_DESC_BASE;
