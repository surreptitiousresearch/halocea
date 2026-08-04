#pragma once
#include "../ws/ds/dsVECTOR.h"
#include "../ws/ds/dsTSTRING.h"
#include "HCEX_MODEL_REGION.h"
// HCEX bridge: a resolved Halo model instance — its ids/names plus per-node and per-region data.
// DB-verified layout (types_members HCEX_MODEL):
//   modelId@0, haloName@4, hcexName@8, objNames@12, nodeIndices@32, regions@52 — size 72.

typedef struct HCEX_MODEL {
    int                            modelId;     // 0x00
    dsTSTRING<char>                haloName;    // 0x04
    dsTSTRING<char>                hcexName;    // 0x08
    dsVECTOR<dsTSTRING<char>, 8>   objNames;    // 0x0C
    dsVECTOR<short, 8>             nodeIndices; // 0x20
    dsVECTOR<HCEX_MODEL_REGION, 8> regions;     // 0x34

    // 0x823D9834 — default-construct: haloName/hcexName adopt the shared empty-string singleton,
    // objNames/nodeIndices/regions adopt the ds_vector.h:24 call-site cookie (empty vectors).
    HCEX_MODEL();
    // 0x823E68F4 — copy-construct: share that.haloName's and that.hcexName's buffers (ref-bump
    // each), then deep-copy objNames/nodeIndices/regions via Insert (adopting each source vector's
    // call-site cookie). No scalar deleting destructor is emitted for this class in the binary.
    HCEX_MODEL(const HCEX_MODEL &that);
    // 0x823D9980 — destroy regions, free nodeIndices' backing store, destroy objNames, then
    // release hcexName's and haloName's buffer references.
    ~HCEX_MODEL();
} HCEX_MODEL;
