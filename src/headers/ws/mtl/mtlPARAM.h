#pragma once
// ws-engine mtl: per-material parameter block. DB-verified layout (types_members mtlPARAM,
// mtlPARAM::BASE / mtlPARAM::LM / mtlPARAM::BASE_LAYER) — a struct wrapping one anonymous-union
// member ___u0 (_6F82C50636646745686A929AF4F34EF5) of three alternative parameter blocks, all
// <= 40 bytes, selected by the material's mtlGlobalType.
// The DB nests BASE/LM/BASE_LAYER inside mtlPARAM; spelled here in a sibling namespace
// (mtlPARAM_arms) since C++ cannot have a namespace and a struct share the name mtlPARAM —
// namespace qualification only, same types. $-name kept verbatim ($ in identifiers is an
// MSVC extension), matching the argb_color.h convention.

namespace mtlPARAM_arms {

// DB types_enum_values mtlPARAM::BASE::GLOBAL_MTL_TYPE.
enum GLOBAL_MTL_TYPE : int {
    GMT_NONE    = 0,
    GMT_TERRAIN = 1,
    GMT_MAX     = 2,
};

typedef struct BASE { // DB types_members mtlPARAM::BASE
    GLOBAL_MTL_TYPE mtlGlobalType;     // 0x00
    char            mtlNameGlobal[16]; // 0x04
    char            mtlName[16];       // 0x14
    unsigned int    tint;              // 0x24
} BASE; // size 0x28 (40)

typedef struct LM { // DB types_members mtlPARAM::LM
    char  source;        // 0x00
    char  _pad01[3];     // 0x01
    float maxIntensity;  // 0x04
    char  lmToolTexInd;  // 0x08
} LM; // DB size 12 (padded to union alignment)

typedef struct BASE_LAYER { // DB types_members mtlPARAM::BASE_LAYER
    char         mtlName[16]; // 0x00
    char         blend;       // 0x10
    char         alphaSource; // 0x11
    char         colorCompNmb;// 0x12
    char         _pad13[1];   // 0x13
    unsigned int tint;        // 0x14
} BASE_LAYER; // size 24

} // namespace mtlPARAM_arms

// DB union _6F82C50636646745686A929AF4F34EF5 — the three parameter shapes overlaid.
typedef union _6F82C50636646745686A929AF4F34EF5 {
    mtlPARAM_arms::BASE       base;       // 0x00
    mtlPARAM_arms::LM         lm;         // 0x00
    mtlPARAM_arms::BASE_LAYER base_layer; // 0x00
} _6F82C50636646745686A929AF4F34EF5; // size 40

typedef struct mtlPARAM {
    _6F82C50636646745686A929AF4F34EF5 ___u0; // 0x00
} mtlPARAM; // size 40
