#pragma once
#include <stdint.h>
// ws-engine vid/d3d: packed per-pass draw record (shader-descriptor / render-state-block /
// fvf indices). DB-verified layout (types_members d3dOBJ_PASS): sdrDesc@0, rsBlock@2, fvfNmb@4,
// id@6, rendFlags@7, sdrPair@8 — size 10. Member widths are the DB's fixed-width spellings
// (unsigned __int16 / unsigned __int8), written stdint per the project convention.

typedef struct d3dOBJ_PASS {
    uint16_t sdrDesc;   // 0x00 shader-descriptor index
    uint16_t rsBlock;   // 0x02 render-state block index
    uint16_t fvfNmb;    // 0x04 fvf descriptor number
    uint8_t  id;        // 0x06 pass id
    uint8_t  rendFlags; // 0x07
    uint16_t sdrPair;   // 0x08 shader-pair index
} d3dOBJ_PASS;          // 10 bytes
