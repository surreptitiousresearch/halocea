#pragma once
#include "../ap/apSTATE_T.h"
#include "mtlPARAM.h"
#include "../../txm_boundary.h"
#include "../txm/txmTEXTURE_PTR.h" // txmTEXTURE_PTR (canonical single definition; do not redefine)
// ws-engine mtl: a single resolved material slot held by mtlTBL::mtlList. DB-verified
// layout (types_members mtlMTL) — size 60 (0x3C).

typedef struct mtlMTL {
    char mtl;     // 0x00 — material tag index; the key FindMtl/_FindMtl search on
    char tcInd;   // 0x01
    char tcInd2;  // 0x02
    char vcInd;   // 0x03
    char vcInd2;  // 0x04
    char tsInd;   // 0x05
    char tsInd2;  // 0x06
    apSTATE_T<unsigned char> state; // 0x07
    txmTEXTURE_PTR pTex;   // 0x08
    txmTEXTURE_PTR pTex2;  // 0x0C
    txmTEXTURE_PTR pTexNM; // 0x10
    mtlPARAM param;  // 0x14 (40 bytes)

    ~mtlMTL(); // boundary — not decompiled here
} mtlMTL; // size 0x3C (60)
