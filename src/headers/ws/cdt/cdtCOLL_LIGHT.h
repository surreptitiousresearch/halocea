#pragma once
#include "voxGRID.h"
#include "../ds/dsVECTOR.h"
// ws-engine cdt: spatial acceleration structure mapping voxel cells to the light IDs that
// touch them (a cdtCOLL specialized for lights). DB-verified layout: cdtCOLL_LIGHT embeds a
// single 204-byte cdtCOLL base; the members the rend light-set update reads live in cdtCOLL —
// voxGrid@4, startIDList@52, idList@72. Per the project's boundary conventions only those are
// typed; the remaining cdtCOLL tail is padding at its exact DB offset/size.

typedef struct cdtCOLL_LIGHT {
    void                        *__vftable;    // 0x00 cdtCOLL_vtbl*
    voxGRID                      voxGrid;      // 0x04 (48)
    dsVECTOR<unsigned int, 8>    startIDList;  // 0x34 (20) per-cell start index into idList (top bit is a flag)
    dsVECTOR<unsigned short, 8>  idList;       // 0x48 (20) flattened light-ID buckets
    unsigned char                _pad5C[112];  // 0x5C .. 0xCC (204) remaining cdtCOLL state  boundary
} cdtCOLL_LIGHT;
