#pragma once
#include "../ds/dsVECTOR.h"
// ws-engine obj: snapshot iterator over an objOBJ's direct children. The constructor copies
// the live child pointers into `childList` so the caller can mutate the tree (e.g. deep-copy
// each child) while iterating. DB-verified layout (types_members objOBJ_ITER_SAFE) — size 24.

struct objOBJ; // ../obj/objOBJ.h — pointer only

typedef struct objOBJ_ITER_SAFE {
    dsVECTOR<objOBJ *, 8> childList; // 0x00 snapshot of the children to visit
    int                   cur;       // 0x14 current index into childList

    // Snapshot `root`'s direct children into childList — body external to this batch.
    objOBJ_ITER_SAFE(objOBJ *root); // boundary
} objOBJ_ITER_SAFE;
