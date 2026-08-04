#pragma once
#include "hcex_matr4x3.h"
// HCEX bridge: per-first-person-model synced transform block embedded in HCEX_ANIM_MNG_FP.
// DB-verified layout (types_members hcex_inst_sync_data): obj_matr@0 (hcex_matr4x3, 52 bytes),
// node_matrices@52 (hcex_matr4x3[64], 3328 bytes), permutations@3380 (unsigned char[8]),
// node_count@3388 (int) — size 3392.

typedef struct hcex_inst_sync_data
{
    hcex_matr4x3  obj_matr;             /* 0x000 */
    hcex_matr4x3  node_matrices[64];    /* 0x034 per-node transform synced from Blam */
    unsigned char permutations[8];      /* 0xD34 */
    int           node_count;           /* 0xD3C */
} hcex_inst_sync_data;
