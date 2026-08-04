#pragma once
#include "../mtl/mtlMTL.h" // txmTEXTURE_PTR
// ws-engine scorch (scrSCORCH_MANAGER): the resolved texture set for one scorch-mark alias
// (diffuse/normal/detail-normal/height/cube maps). DB-verified layout (types_members
// scrTEXTURES): dif@0, nm@4, detNM@8, hm@12, cube@16 (each txmTEXTURE_PTR, 4B) — size 20.

// txmManager (extern txmMANAGER*) comes from txm_boundary.h via mtlMTL.h — do not redeclare
// with a different type (was `void*`, which conflicts with the canonical txmMANAGER* decl).

typedef struct scrTEXTURES {
    txmTEXTURE_PTR dif;   // 0x00 diffuse
    txmTEXTURE_PTR nm;    // 0x04 normal map
    txmTEXTURE_PTR detNM; // 0x08 detail normal map
    txmTEXTURE_PTR hm;    // 0x0C height map
    txmTEXTURE_PTR cube;  // 0x10 cube map

    scrTEXTURES() {}
    scrTEXTURES(const scrTEXTURES &other); // boundary — external to this batch

    // 0x82726F40 — release every non-null texture pointer (cube, hm, detNM, nm, dif — in that
    // order) via txmTEXTURE::Release, but only while txmManager is still alive (skips releasing
    // during/after texture-manager teardown).
    ~scrTEXTURES();

    scrTEXTURES &operator=(const scrTEXTURES &other); // boundary — external to this batch
} scrTEXTURES;
