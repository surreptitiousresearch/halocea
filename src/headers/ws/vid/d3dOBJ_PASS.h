#pragma once
// ws-engine vid/d3d: packed per-pass draw record (shader-descriptor / render-state-block /
// fvf indices). DB-verified layout (types_members d3dOBJ_PASS): sdrDesc@0, rsBlock@2, fvfNmb@4,
// id@6, rendFlags@7, sdrPair@8 — size 10.

typedef struct d3dOBJ_PASS {
    unsigned short sdrDesc;   // 0x00 shader-descriptor index
    unsigned short rsBlock;   // 0x02 render-state block index
    unsigned short fvfNmb;    // 0x04 fvf descriptor number
    unsigned char  id;        // 0x06 pass id
    unsigned char  rendFlags; // 0x07
    unsigned short sdrPair;   // 0x08 shader-pair index
} d3dOBJ_PASS;                // 10 bytes
