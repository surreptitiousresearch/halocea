#pragma once
#include "../ds/dsVECTOR.h"
#include "../ds/dsDATA.h"
// ws-engine ps: the ref-counted backing store of a psARRAY — a growable vector of dsDATA elements.
// DB-verified layout (types_members psARRAY_DATA): refCount@0, data@4 (dsVECTOR<dsDATA,8>) — size 24.

typedef struct psARRAY_DATA {
    int                  refCount; // 0x00
    dsVECTOR<dsDATA, 8>  data;     // 0x04 element storage

    // 0x827AFDD8 — replace the element storage with `num` copies from `arr` (clear, reserve,
    // insert, then shrink capacity to the live count).
    void SetElements(const dsDATA *arr, int num);
} psARRAY_DATA;
