#pragma once
// ws-engine vid: cached occlusion-query resource handles for one object split (depth pass
// and color pass each get a pair of handles, e.g. double-buffered query objects).
// DB-verified layout (types_members vidOBJ_SPLIT_OCC_RES): depth@0 (unsigned int[2]),
// color@8 (unsigned int[2]) — size 16.

typedef struct vidOBJ_SPLIT_OCC_RES {
    unsigned int depth[2]; // 0x00
    unsigned int color[2]; // 0x08
} vidOBJ_SPLIT_OCC_RES;
