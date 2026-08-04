#pragma once
#include "scrSCORCH_MARK_BASE.h"
// ws-engine scorch: a dynamic scorch mark (lives in scrSCORCH_MANAGER::dynScorchList).
// DB-verified layout (types_members scrSCORCH_MARK_DYN) -- size 84.

struct scrBATCH_DYN; // scrBATCH_DYN.h -- boundary (pointer only)

typedef struct scrSCORCH_MARK_DYN : scrSCORCH_MARK_BASE {
    scrBATCH_DYN       *pBatch; // 0x50
} scrSCORCH_MARK_DYN;
